// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCustomWidget.h"
#include "W_CustomMap.h"
#include "Components/TextBlock.h"
#include "W_ItemSlot.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/UserWidget.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanelSlot.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include "WH_BookshelfGimmick.h"
#include "Kismet/GameplayStatics.h"
#include "NewGridWidget.h"
#include "Input/Reply.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Border.h>
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/Delegate.h>
#include "MyDragDropOperation.h"
#include "ItemObject.h"

bool UMapCustomWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!gWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("gWidget is null"));
	}

	if (!itemSlotWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("itemSlotWidgetClass is null"));
	}


	return true;
}

void UMapCustomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!gWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("gWidget is null"));
		return;
	}


	if (!itemSlotWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("itemSlotWidgetClass is null"));
		return;
	}

	NewGridWidget = Cast<UNewGridWidget>(CreateWidget(GetWorld(), gWidget));

	if (!NewGridWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create CustomMapGridWidget"));
		return;
	}

	InitializeItemSlots();

}

bool UMapCustomWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	auto MousePosition = FSlateApplication::Get().GetCursorPos();

	UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop Abs Pos X : %f, Y : %f"), MousePosition.X, MousePosition.Y);

	// UDragDropOperation 객체에서 드래그된 아이템 데이터를 가져옴
	if (UMyDragDropOperation* ItemOperation = Cast<UMyDragDropOperation>(InOperation))
	{
		if (ItemOperation->draggedItemObj)
		{
			AGimmick* matchingActor = NewGridWidget->FindMatchingActor(ItemOperation->draggedItemObj);

			if (matchingActor)
			{

				UE_LOG(LogTemp, Warning, TEXT("machinged actor with itemObject"));

				FVector beginLocation = ItemOperation->draggedItemObj->beginLoc;

				matchingActor->SetActorLocation(beginLocation);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No matching actor found for itemObject"));
			}

			return true;
		}
		
		return false;
	}
	return false;
}

void UMapCustomWidget::InitializeItemSlots()
{
	// if(!itemSlotWidget) return;

	UCanvasPanel* rootCanvas = Cast<UCanvasPanel>(GetRootWidget());

	for (int32 i = 0; i < 4; i++) 
	{
		UW_ItemSlot* newItemSlot = CreateWidget<UW_ItemSlot>(GetWorld(), itemSlotWidgetClass);
		
		if (newItemSlot)
		{
			newItemSlot->mapCustomWidget = this;

			UCanvasPanelSlot* canvasSlot = rootCanvas->AddChildToCanvas(newItemSlot);

			if (canvasSlot)
			{
				canvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
				canvasSlot->SetPosition(FVector2D(400.0f, -380.0f + i * 180.0f));


				itemSlotWidgets.Add(newItemSlot);


				switch (i)
				{
				case 0:
					newItemSlot->itemType = 0;
					break;
				case 1:
					newItemSlot->itemType = 1;
					break;
				case 2:
					newItemSlot->itemType = 2;
					break;
				case 3:
					newItemSlot->itemType = 3;
					break;

				}

				newItemSlot->InitializeItemSlot(i);
			}
		}
	}

}


FReply UMapCustomWidget::OnBackgroundBorderMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}
