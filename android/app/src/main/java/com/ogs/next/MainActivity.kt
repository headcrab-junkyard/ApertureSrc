package com.ogs.next

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View

class MainActivity : AppCompatActivity() {

	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		setContentView(R.layout.activity_main)
	}

	fun play(view: View) {
		val playActivity = Intent(this, RenderSurfaceActivity::class.java)
		startActivity(playActivity)
	}
}
