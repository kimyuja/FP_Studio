   // Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"


FIntPoint UItemObject::GetDimensions() const
{
	return dimensions;
}

UMaterialInterface* UItemObject::GetIcon() const
{
    if (bRotated)
    {
        switch (dimensions.Y) // Assuming Dimensions.Y indicates the rotation state
        {
        case 90:
            return iconRotated90;
        case 180:
            return iconRotated180;
        case 270:
            return iconRotated270;
        default:
            return icon;
        }
    }
    else
    {
        return icon;
    }
}
