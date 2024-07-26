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
#include "WH_BroomstickGimmick.h"
#include "WH_PotionGimmick.h"
#include "WH_WitchCauldronGimmick.h"

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


	if (GActorBtn1)
	{
		GActorBtn1->OnClicked.AddDynamic(this, &UMapCustomWidget::OnGActorBtn1Clicked);
	}

	if (GActorBtn2)
	{
		GActorBtn2->OnClicked.AddDynamic(this, &UMapCustomWidget::OnGActorBtn2Clicked);
	}

	if (GActorBtn3)
	{
		GActorBtn3->OnClicked.AddDynamic(this, &UMapCustomWidget::OnGActorBtn3Clicked);
	}

	if (GActorBtn4)
	{
		GActorBtn4->OnClicked.AddDynamic(this, &UMapCustomWidget::OnGActorBtn4Clicked);
	}

	if (gimmickSelectionWidget)
	{
		canvasSlot = Cast<UCanvasPanelSlot>(gimmickSelectionWidget->Slot);
	}

	if (canvasSlot)
	{
		gswPos = canvasSlot->GetPosition();
	}

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
			RemovedItemCheck(ItemOperation->draggedItemObj);

			if (ItemOperation->draggedItemObj->itemActiveType == 2)
			{
				UE_LOG(LogTemp, Warning, TEXT("!!! REMOVED ITEM ACTIVE TYPE IS %d"), ItemOperation->draggedItemObj->itemActiveType);

				gimmickSelectionWidget->bUsedClearGimmick = false;
				gimmickSelectionWidget->UpdateClearBtnState();
			}

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
		//UE_LOG(LogTemp, Warning, TEXT("Time remaining: %d"), timeRemaining);

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

	return FText::FromString(FString::Printf(TEXT("%02d"), minutes));
}

FText UMapCustomWidget::GetFormattedSeconds()
{
	int32 seconds = timeRemaining % 60;

	return FText::FromString(FString::Printf(TEXT("%02d"), seconds));
}

float UMapCustomWidget::GetProgressBarPercent()
{
	return (float)(timeRemaining) / 60.f;
}

void UMapCustomWidget::OnGActorBtn1Clicked()
{
	UE_LOG(LogTemp, Warning, TEXT("btn1 Clicked!"));

	UE_LOG(LogTemp, Warning, TEXT("bBookShelfInWorld value is %d"), gimmickSelectionWidget->bBookShelfInWorld);

	if (!gimmickSelectionWidget->bBookShelfInWorld)
	{
		gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn1->GetName());

		HandleButtonClicked(GActorBtn1, gswPos);
	}
}

void UMapCustomWidget::OnGActorBtn2Clicked()
{
	UE_LOG(LogTemp, Warning, TEXT("btn2 Clicked!"));

	if (!gimmickSelectionWidget->bBroomstickInWorld)
	{
		gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn2->GetName());

		gswPos += FVector2D(0.f, 150.f);
		HandleButtonClicked(GActorBtn2, gswPos);
	}
}

void UMapCustomWidget::OnGActorBtn3Clicked()
{
	UE_LOG(LogTemp, Warning, TEXT("btn3 Clicked!"));

	if (!gimmickSelectionWidget->bPotionInWorld)
	{
		gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn3->GetName());

		gswPos += FVector2D(0.f, 370.f);
		HandleButtonClicked(GActorBtn3, gswPos);
	}
}

void UMapCustomWidget::OnGActorBtn4Clicked()
{
	UE_LOG(LogTemp, Warning, TEXT("btn4 Clicked!"));

	if (!gimmickSelectionWidget->bPotInWorld)
	{
		gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn4->GetName());

		gswPos += FVector2D(0.f, 520.f);
		HandleButtonClicked(GActorBtn4, gswPos);
	}

}

void UMapCustomWidget::HandleButtonClicked(UButton* _ClickedButton, FVector2D _GSWPos)
{
	if (_ClickedButton == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("clicked button is nullptr"));

		return;
	}

	if (gimmickSelectionWidget->GetVisibility() == ESlateVisibility::Visible)
	{

		if (canvasSlot->GetPosition() == _GSWPos)
		{
			gimmickSelectionWidget->SetVisibility(ESlateVisibility::Collapsed);
			SetGSWBasicPos();
		}
		else
		{
			canvasSlot->SetPosition(_GSWPos);
			SetGSWBasicPos();
		}

	}
	else if (gimmickSelectionWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		gimmickSelectionWidget->SetVisibility(ESlateVisibility::Visible);
		canvasSlot->SetPosition(_GSWPos);
		SetGSWBasicPos();
	}

}

void UMapCustomWidget::SetGSWBasicPos()
{
	gswPos = FVector2D(620.f, -220.f);
}

void UMapCustomWidget::RemovedItemCheck(UItemObject* _itemObject)
{
	if (_itemObject->itemClass == AWH_BookshelfGimmick::StaticClass())
	{
		gimmickSelectionWidget->bBookShelfInWorld = false;
	}
	else if (_itemObject->itemClass == AWH_BroomstickGimmick::StaticClass())
	{
		gimmickSelectionWidget->bBroomstickInWorld = false;
	}
	else if (_itemObject->itemClass == AWH_PotionGimmick::StaticClass())
	{
		gimmickSelectionWidget->bPotionInWorld = false;
	}
	else if (_itemObject->itemClass == AWH_WitchCauldronGimmick::StaticClass())
	{
		gimmickSelectionWidget->bPotInWorld = false;
	}

}

