    // Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "NewGridWidget.h"


FIntPoint UItemObject::GetDimensions(UItemObject* _itemObj)
{
    // UE_LOG(LogTemp, Warning, TEXT("!!! Dimension XY is %d %d"), dimensions.X, dimension.Y);
    UE_LOG(LogTemp, Warning, TEXT("Rotation Angle: %f"), rotationAngle);

    if (rotationAngle == 0.f || rotationAngle == 180.f)
    {
        return dimensions;
    }
    else if (rotationAngle == 90.f || rotationAngle == 270.f)
    {
        return FIntPoint(dimensions.Y, dimensions.X);
    }
    else
    {
        return FIntPoint(0, 0);
    }
}

UMaterialInterface* UItemObject::GetIcon() const
{
    if (bRotated)
    {
        switch (dimensions.Y) // 돌아가는건 dimensions.Y를 기준으로 확인 가능
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

void UItemObject::RotateGA(UItemObject* _itemObj)
{
    if(_itemObj->rotationAngle == 270.f)
    {
        _itemObj->rotationAngle = 0.f;
    }
    else
    {
        _itemObj->rotationAngle += 90.f;
    }

    UE_LOG(LogTemp, Warning, TEXT("!!!! rotation Angle is %f"), _itemObj->rotationAngle);
}

float UItemObject::IsRotatedGA()
{
    return rotationAngle;
}

void UItemObject::SetDimensions(const FIntPoint& NewDimensions)
{
    // currentDimensions = NewDimensions;
    dimensions = NewDimensions;
    
    /*UNewGridWidget* gridWidget = CreateWidget<UNewGridWidget>(GetWorld());
    if (gridWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("!!!! rreeffrreesshh"));
        gridWidget->Refresh();
    }*/

    UE_LOG(LogTemp, Warning, TEXT("!!!! New Dimensions: X=%d, Y=%d"), dimensions.X, dimensions.Y);
}
