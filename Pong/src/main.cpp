#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../include/util.hpp"
#include "../include/ball.hpp"
#include <cstdio>
#include <cstdlib>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <FMOD/fmod.hpp>
#include "FMOD/FmodManager.h"

int player_score = 0;
int ai_score = 0;

bool p1u, p1d;

// Where it's at, yo
float p1y;
float p1i = 2.0f; //player speed

float cpy, cpi;

#define MAX_CHANNEL 255
#define MASTER_CH	"Master"
#define BGM_CH1		"BGM1"
#define BGM_CH2		"BGM2"
#define BGM_CH3		"BGM3"
#define	FX1_CH		"fx1"
#define FX2_CH		"fx2"
#define MUSIC_FILE	"sounds/music.wav"
#define FX1_FILE	"sounds/bounce.wav"
#define FX2_FILE	"sounds/impact.mp3"
#define FX3_FILE	"sounds/ding.wav"
#define FX4_FILE	"sounds/beep2.wav"

FModManager* fmodmanager = nullptr;
bool FMOD_setup();
void masterVolumeCTRL(const char* id);
void FXVolumeCTRL(const char* id);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		p1u = true;
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		p1u = false;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		p1d = true;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		p1d = false;
	}
}

int main(void)
{
	
	util::Util util;
	pong::Ball ball = pong::Ball(util);

	//set player/computer paddle position
	p1y = 0;
	cpy = 0;

	//set computer speed
	cpi = 1.0f; 

	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(800, 600, "Pong", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return -1;
	}
	glfwSetKeyCallback(window, key_callback);

	//initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init("#version 460"))
	{
		return -1;
	}
	ImGui::StyleColorsDark();

	//TODO:
	//intitalize fmod system 
	FMOD_setup();

	//TODO:
	//load sounds to be played during game play

	//initialize the ball
	ball.start();

	float last_time = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		const float now_time = glfwGetTime();
		const float delta_time = now_time - last_time;
		last_time = now_time;

		//move the ball
		ball.tick();

		//if the ball goes past the computer (player wins)
		if (ball.x >= 1.4f)
		{
			player_score += 1;
			ball.reset();
			fmodmanager->play_sound("fx3", FX1_CH);
		}

		//if the ball goes past the player (computer wins)
		if (ball.x <= -1.4f)
		{
			ai_score += 1;
			ball.reset();
			fmodmanager->play_sound("fx4", FX1_CH);
		}

		//bounce ball off bottom
		if (ball.y >= 1.0f)
		{
			ball.collideUp();
			fmodmanager->play_sound("fx1", FX1_CH);
		}

		//bounce ball off top
		if (ball.y <= -1.0f)
		{
			ball.collideDown();
			fmodmanager->play_sound("fx1", FX1_CH);
		}

		//move the player paddle
		if (p1u && p1y <= 1)
		{
			p1y += p1i * delta_time;
		}

		if (p1d && p1y >= -1)
		{
			p1y -= p1i * delta_time;
		}


		//move the computer paddle (just try match the balls y position)
		if (ball.y > cpy)
		{
			cpy += cpi * delta_time;
		}

		if (ball.y < cpy)
		{
			cpy -= cpi * delta_time;
		}

		//check for collisions with player paddle
		if ((ball.x <= -1.15f) && (ball.x >= -1.25f) && (ball.y <= p1y + 0.15) && (ball.y >= p1y - 0.15))
		{
			ball.collideRight();
			cpi += util.randFloat(0.001f, 0.005f);
			fmodmanager->play_sound("fx2", FX1_CH);
		}

		//check for collisions with computer paddle
		if ((ball.x >= 1.15f) && (ball.x <= 1.25f) && (ball.y <= cpy + 0.15) && (ball.y >= cpy - 0.15))
		{
			ball.collideLeft();
			cpi += util.randFloat(0.001f, 0.005f);
			fmodmanager->play_sound("fx2", FX1_CH);
		}

		//render
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		const float ratio = width / static_cast<float>(height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Draw player paddle
		glPushMatrix();
		glTranslatef(-1.2f, p1y, 0);
		glScalef(0.05f, 0.05f, 1);
		glBegin(GL_QUADS);
		glColor3f(0.f, 0.0f, 0.5f);
		glVertex3f(-0.5f, -2.0f, 0.0f);
		glVertex3f(-0.5f, 2.0f, 0.0f);
		glVertex3f(0.5f, 2.0f, 0.0f);
		glVertex3f(0.5f, -2.0f, 0.0f);
		glEnd();
		glPopMatrix();

		// Draw ball
		glPushMatrix();
		glTranslatef(ball.x, ball.y, 0);
		glScalef(0.05f, 0.05f, 1);
		glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);
		glEnd();
		glPopMatrix();

		// Draw AI paddle
		glPushMatrix();
		glTranslatef(1.2f, cpy, 0);
		glScalef(0.05f, 0.05f, 1);
		glBegin(GL_QUADS);
		glColor3f(0.f, 0.5f, 0.0f);
		glVertex3f(-0.5f, -2.0f, 0.0f);
		glVertex3f(-0.5f, 2.0f, 0.0f);
		glVertex3f(0.5f, 2.0f, 0.0f);
		glVertex3f(0.5f, -2.0f, 0.0f);
		glEnd();
		glPopMatrix();


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//score window
		ImGui::Begin("Current Score");
		ImGui::Text("Player:\n%d", player_score);
		ImGui::SameLine();
		ImGui::Text("AI:\n%d", ai_score);
		ImGui::End();

		//audio settings window
		ImGui::Begin("Audio Settings");
		//your controls here (or as keyboard commands above in callback)
		masterVolumeCTRL("Master_volume_ctrl");
		FXVolumeCTRL("FX_volume_ctrl");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//shutdown FMOD
	fmodmanager->shutdown();
	delete fmodmanager;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

bool FMOD_setup()
{
	bool result;
	fmodmanager = new FModManager();

	result = fmodmanager->Fmod_init(MAX_CHANNEL, FMOD_INIT_NORMAL);
	if (!result)
	{
		return -2;
	}

	result = fmodmanager->create_channel(MASTER_CH);
	result = fmodmanager->create_channel(BGM_CH1);
	result = fmodmanager->create_channel(FX1_CH);

	if (!result)
	{
		return -3;
	}

	result = fmodmanager->set_channel_parent(BGM_CH1, MASTER_CH);
	result = fmodmanager->set_channel_parent(FX1_CH, MASTER_CH);

	if (!result)
	{
		return -4;
	}

	result = fmodmanager->set_channel_vol(MASTER_CH, 0.5f);

	result = fmodmanager->create_sound("bgm1", MUSIC_FILE, FMOD_LOOP_NORMAL);
	result = fmodmanager->create_sound("fx1", FX1_FILE, FMOD_DEFAULT);
	result = fmodmanager->create_sound("fx2", FX2_FILE, FMOD_DEFAULT);
	result = fmodmanager->create_sound("fx3", FX3_FILE, FMOD_DEFAULT);
	result = fmodmanager->create_sound("fx4", FX4_FILE, FMOD_DEFAULT);

	result = fmodmanager->play_sound("bgm1", BGM_CH1);
}

void masterVolumeCTRL(const char* id)
{
	ImGui::BeginChild(id,ImVec2(0,60),true);
	FModManager::CHgroup* channel_group;
	fmodmanager->find_channel_group(MASTER_CH, &channel_group);

	float current_volume;
	bool volume_enabled;
	fmodmanager->get_channel_vol(MASTER_CH, &current_volume);
	fmodmanager->get_channel_group_enabled(MASTER_CH, &volume_enabled);

	current_volume *= 100;
	ImGui::Checkbox("enable##master_volume", &volume_enabled);

	ImGui::SliderFloat("master volume", &current_volume, 0.0f, 100.0f, "%.0f");
	current_volume /= 100;


	fmodmanager->set_channel_vol(MASTER_CH, current_volume);
	//ImGui::SameLine();

	fmodmanager->set_channel_group_enabled(MASTER_CH, volume_enabled);
	ImGui::EndChild();
}

void FXVolumeCTRL(const char* id)
{
	ImGui::BeginChild(id, ImVec2(0, 60), true);

	FModManager::CHgroup* channel_group;

	fmodmanager->find_channel_group(FX1_CH, &channel_group);

	float current_volume;
	bool volume_enabled;

	fmodmanager->get_channel_vol(FX1_CH, &current_volume);
	fmodmanager->get_channel_group_enabled(FX1_CH, &volume_enabled);

	current_volume *= 100;
	ImGui::Checkbox("enable##fx_volume", &volume_enabled);
	ImGui::SliderFloat("fx volume", &current_volume, 0.0f, 100.0f, "%.0f");
	current_volume /= 100;

	fmodmanager->set_channel_vol(FX1_CH, current_volume);
	fmodmanager->set_channel_group_enabled(FX1_CH, volume_enabled);

	ImGui::EndChild();
}