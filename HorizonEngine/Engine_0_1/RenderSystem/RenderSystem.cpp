﻿#include "stdafx.h"
#include "RenderSystem.h"


void RenderSystem::startUp()
{
	auto logsys = Application::getSingleton().getLogSystem();
	logsys->log("RenderSystem.InitGLContext");
	glfwInit();
	this->mRenderWindow = this->createRenderWindow(800,600,"Demo");
	this->setRenderTarget(this->mRenderWindow);

}

void RenderSystem::shutDown()
{
	glfwTerminate();
}

std::shared_ptr<RenderWindow> RenderSystem::getRenderWindow()
{
	return this->mRenderWindow;
}

void RenderSystem::setRenderTarget(const RenderTargetPtr& target)
{
	this->mRenderTarget = target;
}

std::shared_ptr<RenderTarget> RenderSystem::getRenderTarget()
{
	return this->mRenderTarget;
}

void RenderSystem::setViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void RenderSystem::setMaterial(const MaterialPtr& material)
{
	mMaterial = material;
}

void RenderSystem::setViewMatrix(const Matrix4x4& viewMat)
{
	mViewMatrix = viewMat;
}

void RenderSystem::setProjectionMatrix(const Matrix4x4& projMat)
{
	mProjectionMatrix = projMat;
}

void RenderSystem::setModelMatrix(const Matrix4x4 & modelMat)
{
	mModelMatrix = mModelMatrix;
}

void RenderSystem::Clear(bool color, bool depth,bool stencil)
{
	GLbitfield mask = 0;
	if (color) {
		mask = mask | GL_COLOR_BUFFER_BIT;
	}
	if (depth) {
		mask = mask | GL_DEPTH_BUFFER_BIT;
	}
	if (stencil) {
		mask = mask | GL_STENCIL_BUFFER_BIT;
	}
	glClear(mask);
}

void RenderSystem::setClearColor(const Color4f & color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void RenderSystem::render(const RenderOperationPtr& op)
{
	GLenum type = GL_TRIANGLES;
	switch (op->getType())
	{
	case RenderOperation::OperationType::TriangleList:
		type = GL_TRIANGLES;
		break;
	case RenderOperation::OperationType::TriangleStrip:
		type = GL_TRIANGLE_STRIP;
		break;
	case RenderOperation::OperationType::TriangleFan:
		type = GL_TRIANGLE_FAN;
		break;
	case RenderOperation::OperationType::LineList:
		type = GL_LINES;
		break;
	case RenderOperation::OperationType::LineStrip:
		type = GL_LINE_STRIP;
		break;
	case RenderOperation::OperationType::PointList:
		type = GL_POINTS;
		break;
	default:
		break;
	}

	auto program = mMaterial->getProgram();
	assert(program);

	assert(!program->isInvalid());

	glUseProgram(program->getID());
	program->setValue(program->getUniformID("MATRIX_VIEW"), mViewMatrix);
	program->setValue(program->getUniformID("MATRIX_PROJ"), mProjectionMatrix);
	program->setValue(program->getUniformID("MATRIX_MODEL"),mModelMatrix);
	mMaterial->apply();
	glBindVertexArray(op->getData()->getVAO()->getID());
	
	if (op->getData()->UseIndices()) {
		glDrawElements(type, op->getData()->getIndicesCount(), op->getData()->getIndicesType(), nullptr);
	}
	else
	{
		glDrawArrays(type, 0, op->getData()->getVertexCount());
	}
	glBindVertexArray(0);
}


std::shared_ptr<RenderWindow> RenderSystem::createRenderWindow(unsigned int width, unsigned int height, std::string title)
{
	RenderWindow* rw = new RenderWindow(width, height, title);
	return std::shared_ptr<RenderWindow>(rw);
}

