// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCustomWidget.h"
#include "W_CustomMap.h"
#include "Components/TextBlock.h"
#include "W_ItemSlot.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanelSlot.h>

bool UMapCustomWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;
}

void UMapCustomWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// itemSlotWidget = Cast<UW_ItemSlot>(CreateWidget(this, itemSlotWidgetClass));

	InitializeItemSlots();

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

void UMapCustomWidget::InitializeItemSlots()
{	
	// if(!itemSlotWidget) return;

	UCanvasPanel* rootCanvas = Cast<UCanvasPanel>(GetRootWidget());

	for (int32 i = 0; i < 4; i++) {
		UW_ItemSlot* newItemSlot = CreateWidget<UW_ItemSlot>(GetWorld(), itemSlotWidgetClass);
		if (newItemSlot)
		{
			newItemSlot->mapCustomWidget = this;
			// 각 인스턴스에 다른 아이콘과 비용을 설정
			int32 costInt = 3 * (i + 1) - i;
			FString costString = FString::FromInt(costInt);
			newItemSlot->itemCost->SetText(FText::FromString(costString));

			UCanvasPanelSlot* canvasSlot = rootCanvas->AddChildToCanvas(newItemSlot);

			if (canvasSlot)
			{
				canvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
				canvasSlot->SetPosition(FVector2D(550.0f, -300.0f + i * 150.0f));
			}

			itemSlotWidgets.Add(newItemSlot);
		}
	}
}

