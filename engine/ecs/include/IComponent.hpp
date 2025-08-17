//
// Created by Godri on 8/17/2025.
//
#pragma once

class IComponent {
	public:
		virtual ~IComponent() = default;
		virtual void Update() = 0;
};