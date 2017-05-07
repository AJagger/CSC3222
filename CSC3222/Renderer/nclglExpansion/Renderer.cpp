/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class is a heavily modified version of the renderers that some of the graphics tutorials for the ncgl library guide you through making.
* It contains a pipeline which the Draw class places objects to render in and then renders these objects on command. It also deals with
* configuring OpenGL.
* 
* The shader files being used are provided in the tutorials.
* 
* Some settings, in particular the setting of the projMatrix should probably be able to be defined somewhere to allow for larger canvases
* but I never got around to doing that.
*/

#include "Renderer.h"
#include "../../Frameworks/DataArray.cpp" //Temp fix to Linker Errors

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {

	currentShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	if (!currentShader->LinkProgram()) {
		return;
	}

	init = true;

	//The projMatrix defines how much pf the canvas can be seen - Setup for the test level is a 32x20 (16:10) canvas
	projMatrix = Matrix4::Orthographic(-1, 100, 32, 0, 20, 0);

	filtering = true;
	usingDepth = false;
	usingAlpha = false;
	repeating = false;
	blendMode = 0;

	ToggleDepth();
	ToggleAlphaBlend();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer(void) {

}

void Renderer::RenderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());

	UpdateShaderMatrices();

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float *)& projMatrix);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, (float *)& viewMatrix);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	//Cycle through objects in renderPipeline and draw.
	DrawData *drawData = renderPipeline.TryToGetFirst();
	if (drawData != nullptr)
	{
		modelMatrix = Matrix4::Translation(drawData->position) * Matrix4::Rotation(drawData->rotation, Vector3(0, 0, 1)) * Matrix4::Scale(Vector3(1.0f, 1.0f, 1.0f));

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float *)& modelMatrix);

		drawData->objectMesh->Draw(drawData->texture);

		while (renderPipeline.IsNext())
		{
			drawData = renderPipeline.Next();
			if (drawData != nullptr)
			{
				modelMatrix = Matrix4::Translation(drawData->position) * Matrix4::Rotation(drawData->rotation, Vector3(0, 0, 1)) * Matrix4::Scale(Vector3(1.0f, 1.0f, 1.0f));

				glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float *)& modelMatrix);

				drawData->objectMesh->Draw(drawData->texture);
			}
		}
	}

	glUseProgram(0);

	SwapBuffers();

	//clear RenderPipeline
	renderPipeline.FreeAll();
}

void  Renderer::UpdateTextureMatrix(float  value) {
	Matrix4  pushPos = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	Matrix4  popPos = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	Matrix4  rotation = Matrix4::Rotation(value, Vector3(0, 0, 1));
	textureMatrix = pushPos * rotation * popPos;

}

void Renderer::ToggleDepth()
{
	usingDepth = !usingDepth;
	usingDepth ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void Renderer::ToggleAlphaBlend()
{
	usingAlpha = !usingAlpha;
	usingAlpha ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void Renderer::ToggleBlendMode()
{
	blendMode = (blendMode + 1) % 4;

	switch (blendMode) {
	case (0): glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
	case (1): glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR); break;
	case (2): glBlendFunc(GL_ONE, GL_ZERO); break;
	case (3): glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
	};

}

void Renderer::AddToPipeline(Mesh *mesh, GLuint texture, Vector3 position, int rotation)
{
	DrawData *data = renderPipeline.CreateNew();
	data->position = position;
	data->texture = texture;
	data->objectMesh = mesh;
	data->rotation = rotation;
}
