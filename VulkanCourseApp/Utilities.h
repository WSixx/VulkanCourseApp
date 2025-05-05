#pragma once

// Indices (locations) of Queue Families (if they exists at all)
struct QueueFamilyIndices { 
	int graphicsFamily = -1;	// Location of Graphics Queue Family
	
	// Check If queue family are valid
	bool isValid() const
	{
		return graphicsFamily >= 0;
	}
};
