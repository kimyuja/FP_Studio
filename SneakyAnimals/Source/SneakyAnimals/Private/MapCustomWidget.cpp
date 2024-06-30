// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCustomWidget.h"
#include "W_CustomMap.h"
#include "Components/TextBlock.h"

bool UMapCustomWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;
}

void UMapCustomWidget::NativeConstruct()
{
	Super::NativeConstruct();


	/*if (CustomMapWidget != nullptr)
	{
		CustomMapWidget->InitializeWidget(tileSize);

	}*/
}

int32 UMapCustomWidget::maxCostAsInt(int32 cost) const
{
	if (maxCost)
	{
		FString maxCostText = maxCost->GetText().ToString();
		
		return FCString::Atoi(*maxCostText)-cost;

	}

	return 0;
}

void UMapCustomWidget::UpdateMaxCost(int32 newCost)
{
	if (maxCost)
	{
		maxCost->SetText(FText::AsNumber(newCost));
	}
}

