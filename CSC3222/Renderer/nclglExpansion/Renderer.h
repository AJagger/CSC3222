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
#pragma  once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../Frameworks/DataArray.h"

struct DrawData
{
	Mesh *objectMesh;
	GLuint texture;
	Vector3 position;
	int rotation;
};

class  Renderer : public  OGLRenderer {

public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void RenderScene();

	void UpdateTextureMatrix(float rotation);
	void ToggleDepth();
	void ToggleAlphaBlend();
	void ToggleBlendMode();

	void AddToPipeline(Mesh *mesh, GLuint texture, Vector3 position, int rotation);

	DataArray<DrawData> renderPipeline = DataArray<DrawData>();

private:
	bool filtering;
	bool repeating;
	bool usingDepth;
	bool usingAlpha;
	int blendMode;
};