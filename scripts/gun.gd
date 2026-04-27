extends MeshInstance3D

@export var fire_rate: float = 0.15
var can_shoot = true

@onready var timer = $Timer

func _ready():
	timer.wait_time = fire_rate

func shoot():
	if can_shoot:
		can_shoot = false
		timer.start()
		# Muzzle flash effect
		create_muzzle_flash()

func create_muzzle_flash():
	# Simple flash
	var flash = OmniLight3D.new()
	flash.light_color = Color(1, 0.8, 0.2)
	flash.omni_range = 10
	flash.light_energy = 5
	add_child(flash)
	await get_tree().create_timer(0.05).timeout
	flash.queue_free()

func _on_timer_timeout():
	can_shoot = true
