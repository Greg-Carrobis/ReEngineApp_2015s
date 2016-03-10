#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	//Fill vector
	move.push_back(vector3(-4.0f, -2.0f, 5.0f));
	move.push_back(vector3(1.0f, -2.0f, 5.0f));
	move.push_back(vector3(-3.0f, -1.0f, 3.0f));
	move.push_back(vector3(2.0f, -1.0f, 3.0f));
	move.push_back(vector3(-2.0f, 0.0f, 0.0f));
	move.push_back(vector3(3.0f, 0.0f, 0.0f));
	move.push_back(vector3(-1.0f, 1.0f, -3.0f));
	move.push_back(vector3(4.0f, 1.0f, -3.0f));
	move.push_back(vector3(0.0f, 2.0f, -5.0f));
	move.push_back(vector3(5.0f, 2.0f, -5.0f));
	move.push_back(vector3(1.0f, 3.0f, -5.0f));

	//vector.size was giving me an error for some reason so I just used a number 
	m_pMatrix = new matrix4();
	m_pMatrix = new matrix4[11];
	m_pSphere = new PrimitiveClass[11];

	//start off at 0, destination being 1
	v3Start = move.at(0);
	v3End = move.at(nextPos);

	//make the spheres
	for (uint i = 0; i < 11; i++)
	{
		m_pSphere[i].GenerateSphere(0.1f, 5, RERED);
		m_pMatrix[i] = glm::translate(move.at(i));
	}

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
		//map the run time to the duration so we can make it animate in the 1 second duration
		float fPercent = MapValue(
			static_cast<float>(fRunTime),
			0.0f,
			(fDuration),
			0.0f,
			1.0f
			);
		//translate the wall-eye by however far I am between start and end in my lerp
		v3Current = glm::lerp(v3Start, v3End, fPercent);
		m_pPosMat = glm::translate(v3Current);
	
		//whenever a second has passed and our wall-eye has lerped
		if (fRunTime > fDuration)
		{
			//at first I nextPos++ after setting v3Start to v3End so it "stuck" for a second every 10 lerps
			nextPos++;
			v3Start = v3End;
			//move along until we hit the end
			if (nextPos < 11)
			{
				v3End = move.at(nextPos);
			}
			else
			{
				//lerp back to the beginning once we've hit the end
				nextPos = 0;
				v3End = move.at(nextPos);
			}
			//reset the run time so we can still know when a second has passed
			fRunTime = 0;
		}
	

	m_pMeshMngr->SetModelMatrix(m_pPosMat, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	//use camera's projection and view
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	//render all of our spheres
	for (int i = 0; i < 11; i++)
	{
		m_pSphere[i].Render(m4Projection, m4View, m_pMatrix[i]);
	}

	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	if (m_pSphere != nullptr)
	{
		delete[] m_pSphere;
		m_pSphere = nullptr;
	}
	if (m_pMatrix != nullptr)
	{
		delete[] m_pMatrix;
		m_pMatrix = nullptr;
	}
	super::Release(); //release the memory of the inherited fields
}