package com.ogs.next

class EngineInterface {

	external fun init()
	external fun resize(newWidth: Int, newHeight: Int)
	external fun render()

	companion object {
		init {
			System.loadLibrary("engine")
		}
	}
}