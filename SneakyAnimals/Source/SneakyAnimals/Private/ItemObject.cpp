    // Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"


FIntPoint UItemObject::GetDimensions() const
{
    // UE_LOG(LogTemp, Warning, TEXT("!!! Dimension XY is %d %d"), dimensions.X, dimension.Y);
	return dimensions;
}

UMaterialInterface* UItemObject::GetIcon() const
{
    if (bRotated)
    {
        switch (dimensions.Y) // ���ư��°� dimensions.Y�� �������� Ȯ�� ����
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
