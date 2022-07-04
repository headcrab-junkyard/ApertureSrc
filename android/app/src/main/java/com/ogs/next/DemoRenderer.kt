package com.ogs.next

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class DemoRenderer : GLSurfaceView.Renderer {

	override fun onDrawFrame(gl: GL10) {
		EngineInterface.render()
	}

	override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
		EngineInterface.resize(width, height)
	}

	override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
		EngineInterface.init()
	}
}