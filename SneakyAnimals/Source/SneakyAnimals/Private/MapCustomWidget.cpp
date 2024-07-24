// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCustomWidget.h"
#include "W_CustomMap.h"
#include "Components/TextBlock.h"
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
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "GimmickSelectionWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include <../../../../../../../Source/Runtime/CoreUObject/Public/UObject/UObjectBaseUtility.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Widget.h>
#include <../../../../../../../Source/Runtime/Core/Public/Math/MathFwd.h>
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UMapCustomWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!gWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("gWidget is null"));
	}

	return true;
}

void UMapCustomWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	StartTimer();

	bIsMouseInsideWidget = false;
	bIsMouseInsideButton = false;

	if (!gWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("gWidget is null"));
		return;
	}

	NewGridWidget = Cast<UNewGridWidget>(CreateWidget(GetWorld(), gWidget));

	if (!NewGridWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create CustomMapGridWidget"));
		return;
	}

	BindButtonEvent();


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

//void UMapCustomWidget::InitializeItemSlots()
//{
//	// if(!itemSlotWidget) return;
//
//	UCanvasPanel* rootCanvas = Cast<UCanvasPanel>(GetRootWidget());
//
//	for (int32 i = 0; i < 4; i++) 
//	{
//		UW_ItemSlot* newItemSlot = CreateWidget<UW_ItemSlot>(GetWorld(), itemSlotWidgetClass);
//		
//		if (newItemSlot)
//		{
//			newItemSlot->mapCustomWidget = this;
//
//			UCanvasPanelSlot* canvasSlot = rootCanvas->AddChildToCanvas(newItemSlot);
//
//			if (canvasSlot)
//			{
//				canvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
//				canvasSlot->SetPosition(FVector2D(400.0f, -380.0f + i * 180.0f));
//
//
//				itemSlotWidgets.Add(newItemSlot);
//
//
//				switch (i)
//				{
//				case 0:
//					newItemSlot->itemType = 0;
//					break;
//				case 1:
//					newItemSlot->itemType = 1;
//					break;
//				case 2:
//					newItemSlot->itemType = 2;
//					break;
//				case 3:
//					newItemSlot->itemType = 3;
//					break;
//
//				}
//
//				newItemSlot->InitializeItemSlot(i);
//			}
//		}
//	}
//
//}


FReply UMapCustomWidget::OnBackgroundBorderMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return FReply::Handled();
}

void UMapCustomWidget::BindButtonEvent()
{
	TArray<UButton*> Btns = {GActorBtn1, GActorBtn2, GActorBtn3, GActorBtn4};

	for (UButton* Btn : Btns)
	{
		if (Btn)
		{
			Btn->OnHovered.AddDynamic(this, &UMapCustomWidget::OnButtonHovered);
			Btn->OnUnhovered.AddDynamic(this, &UMapCustomWidget::OnButtonUnhovered);
			UE_LOG(LogTemp, Warning, TEXT("my btn name is %s"), *Btn->GetName());
		}
	}
}

void UMapCustomWidget::OnButtonHovered()
{
	bIsMouseInsideButton = true;

	UButton* HoveredButton = nullptr;

	if (GActorBtn1->IsHovered())
	{
		HoveredButton = GActorBtn1;
	}
	else if (GActorBtn2->IsHovered())
	{
		HoveredButton = GActorBtn2;
	}
	else if (GActorBtn3->IsHovered())
	{
		HoveredButton = GActorBtn3;
	}
	else if (GActorBtn4->IsHovered())
	{
		HoveredButton = GActorBtn4;
	}

	if (!HoveredButton)
	{
		return;
	}

	UCanvasPanel* rootCanvas = Cast<UCanvasPanel>(GetRootWidget());

	gimmickSelectionWidget = CreateWidget<UGimmickSelectionWidget>(GetWorld(), GimmickSelectionWidgetClass);

	if (gimmickSelectionWidget)
	{
		UCanvasPanelSlot* canvasSlot = rootCanvas->AddChildToCanvas(gimmickSelectionWidget);

		if (canvasSlot)
		{
			FVector2D SelectWPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

			FVector2D ButtonPosition = HoveredButton->GetCachedGeometry().GetAbsolutePosition();

			canvasSlot->SetAnchors(FAnchors(0.f, 0.f));
			// canvasSlot->SetPosition(ButtonPosition + FVector2D(750.f, -200.f));
			canvasSlot->SetPosition(ButtonPosition + FVector2D(100.f, -100.f));
			canvasSlot->SetSize(FVector2D(350.f, 300.f));


			UE_LOG(LogTemp, Warning, TEXT("gimmickSelectionWidget size: (%f, %f)"), canvasSlot->GetSize().X, canvasSlot->GetSize().Y);

			gimmickSelectionWidget->BindBtnWithActiveType((FName)GetName());

			gimmickSelectionWidget->OnCustomMouseEnter.AddDynamic(this, &UMapCustomWidget::OnSelectionWidgetMouseEnter);

			// 위젯 내에서 마우스가 떠나는 이벤트 처리
			gimmickSelectionWidget->OnCustomMouseLeave.AddDynamic(this, &UMapCustomWidget::OnSelectionWidgetMouseLeave);

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create GimmickSelectionWidget"));
	}

}

void UMapCustomWidget::OnSelectionWidgetMouseLeave(const FPointerEvent& InMouseEvent)
{
	bIsMouseInsideWidget = false;

	// 버튼과 위젯 모두에서 마우스가 벗어났을 때만 위젯 제거
	if (!bIsMouseInsideButton)
	{
		if (gimmickSelectionWidget)
		{
			gimmickSelectionWidget->RemoveFromParent();
			gimmickSelectionWidget = nullptr;
		}
	}
}

void UMapCustomWidget::OnSelectionWidgetMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bIsMouseInsideWidget = true;
}

void UMapCustomWidget::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UMapCustomWidget::UpdateTimer, 1.0f, true);

	if (min)
	{
		min->SetText(GetFormattedMinutes());
	}
	if (sec)
	{
		sec->SetText(GetFormattedSeconds());
	}
	if (timerProgressBar)
	{
		timerProgressBar->SetPercent(GetProgressBarPercent());
	}
}

void UMapCustomWidget::UpdateTimer()
{
	if (timeRemaining > 0)
	{
		timeRemaining--;
		UE_LOG(LogTemp, Warning, TEXT("Time remaining: %d"), timeRemaining);

		if (min)
		{
			min->SetText(GetFormattedMinutes());
		}
		if (sec)
		{
			sec->SetText(GetFormattedSeconds());
		}
		if (timerProgressBar)
		{
			timerProgressBar->SetPercent(GetProgressBarPercent());
		}
	}
	else
	{
		TimerFinished();
	}
}

void UMapCustomWidget::TimerFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

	RemoveFromParent();

	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		// 입력 모드를 게임 전용으로 설정
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(pc);

		// 마우스 커서 표시 비활성화
		pc->bShowMouseCursor = false;
	}

}

FText UMapCustomWidget::GetFormattedMinutes()
{
	int32 minutes = timeRemaining / 60;
	// return FText::AsNumber(minutes);
	return FText::FromString(FString::Printf(TEXT("%02d"), minutes));
}

FText UMapCustomWidget::GetFormattedSeconds()
{
	int32 seconds = timeRemaining % 60;
	// return FText::AsNumber(seconds);
	return FText::FromString(FString::Printf(TEXT("%02d"), seconds));
}

float UMapCustomWidget::GetProgressBarPercent()
{
	return (float)(timeRemaining) / 60.f;
}

void UMapCustomWidget::OnButtonUnhovered()
{
	bIsMouseInsideButton = false;

	/*if (!bIsMouseInsideButton)
	{
		if (gimmickSelectionWidget)
		{
			gimmickSelectionWidget->RemoveFromParent();
			gimmickSelectionWidget = nullptr;
		}
	}*/
}