#include "FlotillaNet.h"
#include "ArmadaEngine.h"

#include <glm/glm/glm.hpp>

#include "FlotillaClient.h"
#include "Packet.h"
#include <thread>
//#include <cstdio>
#include <iostream>
#include <sstream>

#include <vector>
#include <algorithm> 

#include "Shader.h"
#include "Model.h"
constexpr int screenWidth = 256;
constexpr int screenHeight = 256;
constexpr int mapWidth = 256;
constexpr int mapHeight = 256;

glID frameBufferID;
glID frameTextureID;
glID renderBufferID;


std::vector<RadioPoint> radarScans;

int currentSequence;
bool setupRenderSystem();
bool shutdownRenderSystem();
void processInput(GLFWwindow *window, FlotillaClient& client);
void processMessage(uint8_t* bufferArray, int size, FlotillaClient& client);
Address parseServerAddress(int argc, char* argv[]);

float currX;
float currY;
bool showSelf = true;
int main(int argc, char* argv[])
{
	socketSetup();
	
	uint8_t messageBuffer[1536];
	
	ArmadaEngine engine{};
	engine.setupWindow(256,256,"Flotilla Engine");
	FreeConsole();

	Address serverLocation = parseServerAddress(argc, argv);
	FlotillaClient client{ serverLocation };


	ShaderElement radarVS("VS/Radar.vs", GL_VERTEX_SHADER);
	ShaderElement radarFS("FS/Radar.fs", GL_FRAGMENT_SHADER);

	Shader radarShader;

	radarShader.addElement(radarVS);
	radarShader.addElement(radarFS);
	radarShader.prime();

	ConnectRequest connect;
	Movement move;
	move.direction = Movement::NONE;
	for (int i = 0; i < 5; i++)
	{
		client.send(&connect);
		client.send(&move);
	}

	radarScans = std::vector<RadioPoint>();

	Vertex points[4];
	{
		points[0].position = glm::vec3(1.0f, 1.0f, 0.0f);
		points[1].position = glm::vec3(1.0f, -1.0f, 0.0f);
		points[2].position = glm::vec3(-1.0f, 1.0f, 0.0f);
		points[3].position = glm::vec3(-1.0f, -1.0f, 0.0f);

		points[0].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		points[1].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		points[2].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		points[3].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		points[0].uv = glm::vec2(0.0f, 0.0f);
		points[1].uv = glm::vec2(1.0f, 0.0f);
		points[2].uv = glm::vec2(0.0f, 1.0f);
		points[3].uv = glm::vec2(1.0f, 1.0f);
	}
	unsigned int indi[]{
		0, 1, 3, 2, 0, 3
	};
	Mesh box(points, 4, indi, 6);

	Model boxMod;
	boxMod.mesh = &box;
	boxMod.transform = glm::mat4{ 1 };

	Address serverCheck;
	float blipDecay = 0.125f;
	while (!glfwWindowShouldClose(engine.currentWindow()))
	{
		processInput(engine.currentWindow(), client);

		//render points

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

		glBlendEquation(GL_MAX);


		radarShader.use();
		radarShader.setFloat("screenWidth", screenWidth);
		radarShader.setFloat("screenHeight", screenHeight);

		auto hitToRadius = [](float hit)
		{
			return hit/2;
		};

		auto hitToAlpha = [](float hit)
		{
			return glm::vec4(1.0f, hit / 20, hit / 20, hit / 20);
		};

		if (showSelf)
		{
			radarShader.setFloat("x", 0);
			radarShader.setFloat("y", 0);
			radarShader.setFloat("radius", 40);
			radarShader.setVec4("alpha", 1, 1, 1, 1);
			radarShader.setVec4("color", 0, 0.25, 0.25, 1);

			boxMod.Draw(radarShader);
		}

		radarShader.setVec4("color", 1,1,1,1);

		for (RadioPoint& rp : radarScans)
		{
			glm::vec4 alpha = hitToAlpha(rp.hit);
			radarShader.setFloat("x", (float)(rp.x - currX));
			radarShader.setFloat("y", (float)(rp.y - currY));
			radarShader.setFloat("radius", hitToRadius(rp.hit));
			radarShader.setVec4("alpha", alpha.r, alpha.g, alpha.b, alpha.a);

			boxMod.Draw(radarShader);
			rp.hit -= blipDecay;
		}
		auto isDecayed = [](RadioPoint rp)
		{
			return rp.hit <= 0;
		};
		radarScans.erase(std::remove_if(radarScans.begin(), radarScans.end(), isDecayed), radarScans.end());
		radarScans.shrink_to_fit();
		//std::cout << radarScans.size() << std::endl;



		int messageSize = client.clientSocket.receive(serverCheck, messageBuffer, sizeof(messageBuffer));
		while (messageSize > 0 && serverCheck == serverLocation)
		{
			processMessage(messageBuffer, messageSize, client);
			messageSize = client.clientSocket.receive(serverCheck, messageBuffer, sizeof(messageBuffer));
		}


		glfwPollEvents();
		glfwSwapBuffers(engine.currentWindow());
		Sleep(100/6);
	}

	Disconnect disconnect;
	disconnect.protocol = protocolID;
	disconnect.command = CommandType::Disconnect;
	for (int i = 0; i < 5; i++)
	{
		client.send(&disconnect);
	}
	socketShutDown();
	return 0;
}

void processMessage(uint8_t* bufferArray, int size, FlotillaClient& client)
{
	Buffer reader;
	reader.data = bufferArray;
	reader.index = 0;
	reader.size = size;
	Packet packetHead;
	packetHead.Read(reader);

	if (packetHead.command == CommandType::ConnectReply)
	{
		reader.index = 0;
		ConnectReply connectPacket;
		connectPacket.Read(reader);
		if (connectPacket.accepted)
		{
			//Connection Established
		}
		else
		{
			//Server access denied
		}
	}
	else if (packetHead.command == CommandType::MoveACK)
	{
		reader.index = 0;
		MoveACK movePacket;
		movePacket.Read(reader);
		currX = movePacket.x;
		currY = movePacket.y;
	}
	else if (packetHead.command == CommandType::Radio)
	{
		reader.index = 0;
		Radio radioPacket;
		radioPacket.Read(reader);


		//PROCESS RADIO SIGNALS
		auto sameSpace = [](RadioPoint a, RadioPoint b) {
			return ((int)(a.x) == (int)(b.x)) && ((int)(a.y) == (int)(b.y));
		};

		currentSequence = radioPacket.sequence;
		radarScans.insert(radarScans.end(),radioPacket.points, radioPacket.points + radioPacket.count);
		std::unique(radarScans.rbegin(), radarScans.rend(), sameSpace);


		//ACK Radio Signals
		RadioACK reply;
		reply.count = radioPacket.count;
		reply.sequence = radioPacket.sequence;
		client.send(&reply);
	}
}


void processInput(GLFWwindow *window, FlotillaClient &client)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	short dx = 0;
	short dy = 0;

	showSelf = true;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		showSelf = false;


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		dy++;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		dy--;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		dx--;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		dx++;
	if (dx != 0 || dy != 0)
	{
		Movement moveReport;

		switch (dx+16*dy)
		{
		case -1:
			moveReport.direction = Movement::LEFT;
			break;
		case 1:
			moveReport.direction = Movement::RIGHT;
			break;
		case -16:
			moveReport.direction = Movement::DOWN;
			break;
		case 16:
			moveReport.direction = Movement::UP;
			break;
		case -17:
			moveReport.direction = Movement::DL;
			break;
		case -15:
			moveReport.direction = Movement::DR;
			break;
		case 15:
			moveReport.direction = Movement::UL;
			break;
		case 17:
			moveReport.direction = Movement::UR;
			break;
		default:
			moveReport.direction = Movement::NONE;
			break;
		}
		client.send(&moveReport);
	}

}


bool setupRenderSystem()
{
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	glGenTextures(1, &frameTextureID);
	glBindTexture(GL_TEXTURE_2D, frameTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//attach
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTextureID, 0);

	glGenRenderbuffers(1, &renderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//attach
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);
	return true;
}

bool shutdownRenderSystem()
{
	glDeleteFramebuffers(1, &frameBufferID);
	return true;
}



Address parseServerAddress(int argc, char* argv[])
{
	Address defaultAddress{ 127,0,0,1,30000 };
	if (argc == 1)
	{
		std::cout << "....No Address specified.\n\tDefaulting to 127.0.0.1 30000\n";
		return defaultAddress;
	}
	std::istringstream ss1(argv[1]);
	std::string part;

	int ipParts = 0;
	unsigned short ipChain[4] = { 127,0,0,1 }; //short to make istringstream work
	unsigned short portChain = 30000;
	bool ipSuccess = false;
	while (ipParts < 4 && std::getline(ss1, part, '.'))
	{
		std::istringstream extract(part);
		if (!(extract >> (ipChain[ipParts]) && (ipChain[ipParts] < 256)))
		{
			//Failure. Abort.
			std::cout << "!!!!IP Address parse failed.\n\tDefaulting to 127.0.0.1\n";
			ipChain[0] = static_cast<unsigned char>(127);
			ipChain[1] = static_cast<unsigned char>(0);
			ipChain[2] = static_cast<unsigned char>(0);
			ipChain[3] = static_cast<unsigned char>(1);
			break;
		}
		ipParts++;
		if (ipParts == 4)
		{
			ipSuccess = true;
		}
	}

	std::istringstream ss2;
	if (argc > 2)
	{
		ss2 = std::istringstream(argv[2]);
		if (!(ss2 >> portChain))
		{
			//Failure. Abort.
			std::cout << "!!!!Port Address parse failed.\n\tDefaulting to 30000\n";
			portChain = 30000;
		}
	}

	std::cout << "....Interpreted Server Address:\n\t"
		<< ipChain[0] << "."
		<< ipChain[1] << "."
		<< ipChain[2] << "."
		<< ipChain[3] << " "
		<< portChain << "\n";
	return Address(
		static_cast<unsigned char>(ipChain[0]),
		static_cast<unsigned char>(ipChain[1]),
		static_cast<unsigned char>(ipChain[2]),
		static_cast<unsigned char>(ipChain[3]), portChain);
}