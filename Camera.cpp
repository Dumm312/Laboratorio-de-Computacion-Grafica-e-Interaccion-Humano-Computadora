#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	// Inicializar valores para tercera persona
	cameraDistance = 8.0f;    // Ajusta seg�n necesites
	cameraHeight = 8.0f;      // Ajusta seg�n necesites

	// Inicializar valores para vista a�rea
	isAerialView = false;
	aerialHeight = 500.0f;  // Altura de la vista a�rea
	aerialMoveSpeed = 0.5f;  // Velocidad de movimiento en vista a�rea

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime){
	GLfloat velocity = moveSpeed * deltaTime;
	if (keys[GLFW_KEY_W]){position += front * velocity;}
	if (keys[GLFW_KEY_S]){position -= front * velocity;}
	if (keys[GLFW_KEY_A]){position -= right * velocity;}
	if (keys[GLFW_KEY_D]){position += right * velocity;}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange){
	xChange *= turnSpeed;
	yChange *= turnSpeed;
	yaw += xChange; pitch += yChange;
	if (pitch > 89.0f){pitch = 89.0f;}
	if (pitch < -89.0f){pitch = -89.0f;}
	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update(){
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

// Implementaci�n del m�todo updateThirdPerson
void Camera::updateThirdPerson(const glm::vec3& characterPosition, float characterRotation) {
	// Actualizar la posici�n objetivo (personaje)
	targetPosition = characterPosition;

	// Calcular la posici�n de la c�mara basada en la rotaci�n del personaje
	float angleInRadians = glm::radians(characterRotation);

	// Posicionar la c�mara detr�s del personaje
	position.x = targetPosition.x - (cameraDistance * sin(angleInRadians));
	position.z = targetPosition.z - (cameraDistance * cos(angleInRadians));
	position.y = targetPosition.y + cameraHeight;

	// Hacer que la c�mara mire hacia el personaje
	front = glm::normalize(targetPosition - position);

	// Actualizar los vectores right y up
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::toggleAerialView() {
	isAerialView = !isAerialView;
	if (isAerialView) { // Guardar la posici�n actual antes de cambiar a vista a�rea
		position.y = aerialHeight; // Apuntar directamente hacia abajo
		front = glm::vec3(0.0f, -1.0f, 0.0f);
		right = glm::vec3(1.0f, 0.0f, 0.0f);
		up = glm::vec3(0.0f, 0.0f, -1.0f);
	}
}

void Camera::updateAerialView(bool* keys, GLfloat deltaTime) {
	if (!isAerialView) return;
	GLfloat velocity = aerialMoveSpeed * deltaTime;

	// Movimiento ortogonal en el plano XZ
	if (keys[GLFW_KEY_W]) { position.z -= velocity; }  // Mover hacia adelante en Z
	if (keys[GLFW_KEY_S]) { position.z += velocity; }  // Mover hacia atr�s en Z
	if (keys[GLFW_KEY_A]) { position.x -= velocity; }  // Mover hacia la izquierda en X
	if (keys[GLFW_KEY_D]) { position.x += velocity; }  // Mover hacia la derecha en X
	position.y = aerialHeight;// Mantener la altura constante
}


Camera::~Camera(){}
