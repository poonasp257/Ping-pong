#include "stdafx.h"

Light::Light() {

}

Light::~Light() {

}

void Light::setDiffuseColor(float red, float green, float blue, float alpha) {
	diffuseColor = { red, green, blue, alpha };
}

void Light::setDirection(float x, float y, float z) {
	direction = { x, y, z };
}

void Light::setAmbientColor(float red, float green, float blue, float alpha) {
	ambientColor = { red, green, blue, alpha };
}

void Light::setSpecularColor(float red, float green, float blue, float alpha) {
	specularColor = { red, green, blue, alpha };
}

void Light::setSpecularPower(float power) {
	specularPower = power;
}