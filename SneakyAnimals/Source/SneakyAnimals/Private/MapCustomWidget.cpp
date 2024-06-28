// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCustomWidget.h"
#include "W_CustomMap.h"

bool UMapCustomWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	Super::NativeConstruct();

	return true;
}

void UMapCustomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CustomMapWidget != nullptr)
	{
		CustomMapWidget->InitializeWidget(tileSize);

	}
}

