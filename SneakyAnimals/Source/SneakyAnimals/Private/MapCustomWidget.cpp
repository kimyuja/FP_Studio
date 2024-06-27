// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCustomWidget.h"
#include "W_CustomMap.h"

void UMapCustomWidget::InitializeCustomMap(UClass* customMapClass, UItemComponent* itemComponent, float tileSize)
{
	if (customMapClass != nullptr)
	{
		CustomMapWidget = CreateWidget<UW_CustomMap>(GetWorld(), customMapClass);
		if (CustomMapWidget != nullptr)
		{
			CustomMapWidget->AddToViewport();
			CustomMapWidget->InitializeWidget(itemComponent, tileSize);
		}
	}
}
