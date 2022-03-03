#pragma once

#include "parser.hpp"

class Beatmap
{
	std::vector<Action> actions;

	float overallDifficulty;
	float circleSize;
	float approachRate;
	float sliderMultiplier;

	public:
		Beatmap()
		{
			this->overallDifficulty = -1;
			this->circleSize = -1;
			this->approachRate = -1;
			this->sliderMultiplier = -1;
		}

		/// <summary>
		/// Checks if all fields within the beatmap class are filled, these fields
		/// contain "overallDifficulty", "cirlceSize", "approachRate", "sliderMultiplier"
		/// </summary>
		/// <returns>true if they are filled, false if they are not.</returns>
		bool isFilled()
		{
			return this->overallDifficulty != -1L && this->approachRate != -1L && this->sliderMultiplier != -1L && this->circleSize != -1L;
		}

		std::vector<Action> getActions()
		{
			return this->actions;
		}

		void setActions(std::vector<Action> actions)
		{
			this->actions = actions;
		}

		float getOverallDifficulty()
		{
			return this->overallDifficulty;
		}

		void setOverallDifficulty(float overallDifficulty)
		{
			this->overallDifficulty = overallDifficulty;
		}

		float getCircleSize()
		{
			return this->circleSize;
		}

		void setCircleSize(float circleSize)
		{
			this->circleSize = circleSize;
		}

		float getApproachRate()
		{
			return this->approachRate;
		}

		void setApproachRate(float approachRate)
		{
			this->approachRate = approachRate;
		}

		float getSliderMultiplier()
		{
			return this->sliderMultiplier;
		}

		void setSliderMultiplier(float sliderMultiplier)
		{
			this->sliderMultiplier = sliderMultiplier;
		}
};