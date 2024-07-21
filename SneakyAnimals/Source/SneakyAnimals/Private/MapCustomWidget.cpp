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

	// CreateWidget called as nullptr 오류가 뜬다
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

	// CustomMapGridWidget = Cast<UW_CustomMap>(CreateWidget(this, gWidget));

	NewGridWidget = Cast<UNewGridWidget>(CreateWidget(GetWorld(), gWidget));

	if (!NewGridWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create CustomMapGridWidget"));
		return;
	}

	InitializeItemSlots();

	/*if (NewGridWidget != nullptr)
	{
		NewGridWidget->GridBorderSetSize(tileSize);

	}*/
}

bool UMapCustomWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	auto MousePosition = FSlateApplication::Get().GetCursorPos();

	UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop Abs Pos X : %f, Y : %f"), MousePosition.X, MousePosition.Y);

	// UDragDropOperation 객체에서 드래그된 아이템 데이터를 가져옵니다.
	if (UMyDragDropOperation* ItemOperation = Cast<UMyDragDropOperation>(InOperation))
	{
		if (ItemOperation->draggedItemObj)
		{

			IncreaseCurrentCost(ItemOperation->draggedItemObj);
			UE_LOG(LogTemp, Warning, TEXT("increased Cost"));

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

		//{
		//	UE_LOG(LogTemp, Warning, TEXT("draggedItemObj is not nullptr"));

		//	int32 thisItemCost = ItemOperation->draggedItemObj->itemCost;

		//	_currentCost += thisItemCost;

		//	UpdateMaxCost(_currentCost);

		//	// 비용이 유효한지 확인
		//	if (ValidCost())
		//	{
		//		// SetDtopLoc() 함수 실행
		//		// 서버로 넘기고 -> 멀티로 넘겨서 서버와 클라 모두에게서 setloc 될 수 있도록 rpc 설정해줘야함
		//		// rpc는 컨트롤러를 가진 대상만 사용할 수 있으므로 여기서 위치 옮기는 함수 만들고
		//		// 캐릭터 cpp에서 rpc로 실행시켜줘야 해
		//	}
		//	else
		//	{
		//		UE_LOG(LogTemp, Error, TEXT("Not enough cost to drop the item"));
		//	}

		//	return true;
		
		return false;
	}
	return false;
}

int32 UMapCustomWidget::DecreaseCurrentCost(int32 cost)
{
	if (maxCost)
	{
		FString maxCostText = maxCost->GetText().ToString();

		int32 temp = (int32)FCString::Atoi(*maxCostText) - cost;

		_currentCost = temp;

		if (_currentCost >= 0)
		{
			// ValidCost(true);
			return _currentCost;
		}
		else 
		{
			// ValidCost(false);
		}

	}

	return 0;
}

void UMapCustomWidget::IncreaseCurrentCost(UItemObject* _ItemObj)
{
	int32 thisItemCost = _ItemObj->itemCost;

	_currentCost += thisItemCost;

	UpdateMaxCost(_currentCost);
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

	for (int32 i = 0; i < 4; i++) 
	{
		UW_ItemSlot* newItemSlot = CreateWidget<UW_ItemSlot>(GetWorld(), itemSlotWidgetClass);
		
		if (newItemSlot)
		{
			newItemSlot->mapCustomWidget = this;

			/*int32 costInt = 3 * (i + 1) - i;
			FString costString = FString::FromInt(costInt);
			newItemSlot->itemCost->SetText(FText::FromString(costString));*/

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

	/*UW_ItemSlot* bookShelfBtn = Cast<UW_ItemSlot>(itemSlotWidgets[0]);
	UW_ItemSlot* broomstickBtn = Cast<UW_ItemSlot>(itemSlotWidgets[1]);
	UW_ItemSlot* posionBtn = Cast<UW_ItemSlot>(itemSlotWidgets[2]);
	UW_ItemSlot* cauldronBtn = Cast<UW_ItemSlot>(itemSlotWidgets[3]);*/

	// itemSlotWidgets[0]->itemCost->SetText(FText::FromString("999"));

	// 액터를 월드에서 호출하여 내부에서 작업할 수 있게 만듬
	//AWH_BookshelfGimmick* tempClass = Cast<AWH_BookshelfGimmick>(UGameplayStatics::GetActorOfClass(GetWorld()));

	// 호출된 액터는 해당 슬롯이 눌리면 반응
	//


}

bool UMapCustomWidget::ValidCost()
{
	if (_currentCost >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

FReply UMapCustomWidget::OnBackgroundBorderMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}
