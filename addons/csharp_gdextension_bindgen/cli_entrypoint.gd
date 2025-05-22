extends SceneTree


const CSharpGDExtensionBindgen = preload("csharp_gdextension_bindgen.gd")


func _initialize():
	CSharpGDExtensionBindgen.generate_gdextension_csharp_scripts.callv(OS.get_cmdline_user_args())
	quit()
