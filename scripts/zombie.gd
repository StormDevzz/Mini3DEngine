extends CharacterBody3D

@export var health: int = 50
@export var speed: float = 3.0
@export var damage: int = 10

var player = null
var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")

@onready var mesh = $MeshInstance3D

func _ready():
	add_to_group("enemies")
	# Find player
	await get_tree().create_timer(0.1).timeout
	player = get_tree().get_first_node_in_group("player")

func _physics_process(delta):
	if not is_on_floor():
		velocity.y -= gravity * delta

	if player:
		var direction = (player.global_position - global_position).normalized()
		direction.y = 0
		
		if global_position.distance_to(player.global_position) > 1.5:
			velocity.x = direction.x * speed
			velocity.z = direction.z * speed
			look_at(player.global_position, Vector3.UP)
		else:
			velocity.x = 0
			velocity.z = 0
			attack_player()

	move_and_slide()

func attack_player():
	if player and player.has_method("take_damage"):
		player.take_damage(damage)

func take_damage(amount: int):
	health -= amount
	if health <= 0:
		die()

func die():
	queue_free()
