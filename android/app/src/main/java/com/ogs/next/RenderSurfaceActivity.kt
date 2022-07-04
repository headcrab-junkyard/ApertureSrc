package com.ogs.next

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.opengl.GLSurfaceView
import android.view.Window
import android.view.WindowManager

class RenderSurfaceActivity : AppCompatActivity() {

	private lateinit var surface: GLSurfaceView

	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)

		requestWindowFeature(Window.FEATURE_NO_TITLE)
		window.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN)

		surface = GLSurfaceView(this)
		surface.setEGLContextClientVersion(2)
		//surface.setRenderer(DemoRenderer()) // TODO

		setContentView(surface)
	}

	override fun onPause() {
		super.onPause()
		surface.onPause()
	}

	override fun onResume() {
		super.onResume()
		surface.onResume()
	}
}