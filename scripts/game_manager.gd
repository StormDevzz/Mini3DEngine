extends Node3D

@export var zombie_scene: PackedScene
@export var spawn_interval: float = 2.0
@export var max_zombies: int = 10

var zombies_killed = 0
var score = 0

@onready var spawn_points = $SpawnPoints.get_children()
@onready var timer = $SpawnTimer

func _ready():
	timer.wait_time = spawn_interval
	timer.start()
	spawn_zombie()

func _on_spawn_timer_timeout():
	var zombie_count = get_tree().get_nodes_in_group("enemies").size()
	if zombie_count < max_zombies:
		spawn_zombie()

func spawn_zombie():
	if zombie_scene and spawn_points.size() > 0:
		var spawn_point = spawn_points[randi() % spawn_points.size()]
		var zombie = zombie_scene.instantiate()
		zombie.global_position = spawn_point.global_position
		add_child(zombie)
