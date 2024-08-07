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
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include "TestPlayer.h"
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
#include "Components/WidgetSwitcher.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "WH_BroomstickGimmick.h"
#include "WH_PotionGimmick.h"
#include "WH_WitchCauldronGimmick.h"
#include "SM_ComputerGimmick.h"
#include "SM_PeriscopeGimmick.h"
#include "SM_PressButtonGimmick.h"
#include "SM_WhistleGimmick.h"
#include "SP_BottleGimmick.h"
#include "SP_CartGimmick.h"
#include "SP_CleanerGimmick.h"
#include "SP_ShowcaseGimmick.h"
#include "BS_GoldBarGimmick.h"
#include "BS_HandleGimmick.h"
#include "BS_LaserGimmick.h"
#include "BS_SwitchGimmick.h"
#include <../../../../../../../Source/Runtime/SlateCore/Public/Fonts/SlateFontInfo.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/AudioComponent.h>

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

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (playerCharacter)
	{
		myPlayer = Cast<ATestPlayer>(playerCharacter);
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
		// gswPos = canvasSlot->GetPosition();
		gswPos = FVector2D(620.f, -220.f);
	}
	
	if (playerRandNum != -1)
	{
		BackgroundImgSwitcher->SetActiveWidgetIndex(playerRandNum);

		Btn1WidgetSwitcher->SetActiveWidgetIndex(playerRandNum);
		Btn2WidgetSwitcher->SetActiveWidgetIndex(playerRandNum);
		Btn3WidgetSwitcher->SetActiveWidgetIndex(playerRandNum);
		Btn4WidgetSwitcher->SetActiveWidgetIndex(playerRandNum);
	}

	gimmickSelectionWidget->SetPlayerRandNum(playerRandNum);
	floorTopViewSwitcher->SetActiveWidgetIndex(playerRandNum);

	UE_LOG(LogTemp, Error, TEXT("PLAYER RANDOM NUM IS %d"), playerRandNum);

	if (customUIBgm)
	{
		audioComponent = UGameplayStatics::SpawnSound2D(this, customUIBgm);
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
				FRotator beginRotation = ItemOperation->draggedItemObj->beginRot;

				matchingActor->SetActorLocation(beginLocation);
				matchingActor->SetActorRotation(beginRotation);

				myPlayer->ServerRPC_SetGActorLoc(matchingActor, beginLocation);
				myPlayer->ServerRPC_SetGActorRot(matchingActor, beginRotation);

				NewGridWidget->WhenGADropSetMulti(matchingActor, beginLocation, beginRotation);
				// myPlayer->ServerRPC_SetGActorLoc(matchingActor, beginLocation, );
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

void UMapCustomWidget::NativeDestruct()
{
	if (customUIBgm)
	{
		audioComponent->FadeOut(2.0f, 0.0f);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (audioComponent)
				{
					audioComponent->Stop();
				}
			}, 2.0f, false);
	}
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
	UE_LOG(LogTemp, Warning, TEXT("btn1 Clicked!!!!!"));

	FSlateFontInfo fontInfo1 = gimmickSelectionWidget->GmTxt1->GetFont();
	fontInfo1.Size = 22.f;
	gimmickSelectionWidget->GmTxt1->SetFont(fontInfo1);

	FSlateFontInfo fontInfo2 = gimmickSelectionWidget->GmTxt2->GetFont();
	fontInfo2.Size = 22.f;
	gimmickSelectionWidget->GmTxt2->SetFont(fontInfo2);

	// UE_LOG(LogTemp, Warning, TEXT("bBookShelfInWorld value is %d"), gimmickSelectionWidget->bBookShelfInWorld);

	// 마녀의집
	if (playerRandNum == 0)
	{
		if (!gimmickSelectionWidget->bBookShelfInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn1->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Nearby Players"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			fontInfo1.Size = 13.f;
			gimmickSelectionWidget->GmTxt1->SetFont(fontInfo1);

			HandleButtonClicked(GActorBtn1, gswPos);
		}
	}
	// 잠수함
	else if (playerRandNum == 1)
	{
		if (!gimmickSelectionWidget->bComputerInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn1->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To All"));

			HandleButtonClicked(GActorBtn1, gswPos);
		}
	}
	// 슈퍼마켓
	else if (playerRandNum == 2)
	{
		if (!gimmickSelectionWidget->bBottleInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn1->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			// fontInfo2.Size = 16.f;
			// gimmickSelectionWidget->GmTxt2->SetFont(fontInfo2);

			HandleButtonClicked(GActorBtn1, gswPos);
		}
	}
	// 금고
	else if (playerRandNum == 3)
	{
		if (!gimmickSelectionWidget->bGoldbarInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn1->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			HandleButtonClicked(GActorBtn1, gswPos);
		}
	}
}

void UMapCustomWidget::OnGActorBtn2Clicked()
{

	UE_LOG(LogTemp, Warning, TEXT("btn1 Clicked!"));

	FSlateFontInfo fontInfo1 = gimmickSelectionWidget->GmTxt1->GetFont();
	fontInfo1.Size = 22.f;
	gimmickSelectionWidget->GmTxt1->SetFont(fontInfo1);

	FSlateFontInfo fontInfo2 = gimmickSelectionWidget->GmTxt2->GetFont();
	fontInfo2.Size = 22.f;
	gimmickSelectionWidget->GmTxt2->SetFont(fontInfo2);

	UE_LOG(LogTemp, Warning, TEXT("btn2 Clicked!"));

	// 마녀의집
	if (playerRandNum == 0)
	{
		if (!gimmickSelectionWidget->bBroomstickInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn2->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			gswPos += FVector2D(0.f, 150.f);
			HandleButtonClicked(GActorBtn2, gswPos);
		}
	}
	// 잠수함
	else if (playerRandNum == 1)
	{
		if (!gimmickSelectionWidget->bPeriscopeInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn2->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			gswPos += FVector2D(0.f, 150.f);
			HandleButtonClicked(GActorBtn2, gswPos);
		}
	}
	// 슈퍼마켓
	else if (playerRandNum == 2)
	{
		if (!gimmickSelectionWidget->bCartInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn2->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self")); 
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			gswPos += FVector2D(0.f, 150.f);
			HandleButtonClicked(GActorBtn2, gswPos);
		}
	}
	// 금고
	else if (playerRandNum == 3)
	{
		if (!gimmickSelectionWidget->bHandleInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn2->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To All"));

			gswPos += FVector2D(0.f, 150.f);
			HandleButtonClicked(GActorBtn2, gswPos);
		}
	}

}

void UMapCustomWidget::OnGActorBtn3Clicked()
{
	FSlateFontInfo fontInfo1 = gimmickSelectionWidget->GmTxt1->GetFont();
	fontInfo1.Size = 22.f;
	gimmickSelectionWidget->GmTxt1->SetFont(fontInfo1);

	FSlateFontInfo fontInfo2 = gimmickSelectionWidget->GmTxt2->GetFont();
	fontInfo2.Size = 22.f;
	gimmickSelectionWidget->GmTxt2->SetFont(fontInfo2);

	UE_LOG(LogTemp, Warning, TEXT("btn3 Clicked!"));

	// 마녀의집
	if (playerRandNum == 0)
	{
		if (!gimmickSelectionWidget->bPotionInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn3->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Nearby Players"));

			fontInfo2.Size = 13.f;
			gimmickSelectionWidget->GmTxt2->SetFont(fontInfo2);

			gswPos += FVector2D(0.f, 370.f);
			HandleButtonClicked(GActorBtn3, gswPos);
		}
	}
	// 잠수함
	else if (playerRandNum == 1)
	{
		if (!gimmickSelectionWidget->bPressBtnInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn3->GetName());
	
			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To All"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			gswPos += FVector2D(0.f, 370.f);
			HandleButtonClicked(GActorBtn3, gswPos);
		}
	}
	// 슈퍼마켓
	else if (playerRandNum == 2)
	{
		if (!gimmickSelectionWidget->bCleanerInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn3->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			gswPos += FVector2D(0.f, 370.f);
			HandleButtonClicked(GActorBtn3, gswPos);
		}
	}
	// 금고
	else if (playerRandNum == 3)
	{
		if (!gimmickSelectionWidget->bLaserInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn3->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To All"));

			gswPos += FVector2D(0.f, 370.f);
			HandleButtonClicked(GActorBtn3, gswPos);
		}
	}
}

void UMapCustomWidget::OnGActorBtn4Clicked()
{
	UE_LOG(LogTemp, Warning, TEXT("btn4 Clicked!"));

	FSlateFontInfo fontInfo1 = gimmickSelectionWidget->GmTxt1->GetFont();
	fontInfo1.Size = 22.f;
	gimmickSelectionWidget->GmTxt1->SetFont(fontInfo1);

	FSlateFontInfo fontInfo2 = gimmickSelectionWidget->GmTxt2->GetFont();
	fontInfo2.Size = 22.f;
	gimmickSelectionWidget->GmTxt2->SetFont(fontInfo2);

	// 마녀의집
	if (playerRandNum == 0)
	{
		if (!gimmickSelectionWidget->bPotInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn4->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			gswPos += FVector2D(0.f, 520.f);
			HandleButtonClicked(GActorBtn4, gswPos);
		}
	}
	// 잠수함
	else if (playerRandNum==1)
	{
		if (!gimmickSelectionWidget->bWhistleInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn4->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Self"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			gswPos += FVector2D(0.f, 520.f);
			HandleButtonClicked(GActorBtn4, gswPos);
		}
	}
	// 슈퍼마켓
	else if (playerRandNum == 2)
	{
		if (!gimmickSelectionWidget->bShowcaseInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn4->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To Nearby Players"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			fontInfo1.Size = 13.f;
			gimmickSelectionWidget->GmTxt1->SetFont(fontInfo1);

			gswPos += FVector2D(0.f, 520.f);
			HandleButtonClicked(GActorBtn4, gswPos);
		}
	}
	// 금고
	else if (playerRandNum == 3)
	{
		if (!gimmickSelectionWidget->bSwitchInWorld)
		{
			gimmickSelectionWidget->BindBtnWithActiveType((FName)GActorBtn4->GetName());

			gimmickSelectionWidget->GmTxt1->SetText(FText::FromString("To All"));
			gimmickSelectionWidget->GmTxt2->SetText(FText::FromString("To Self"));

			gswPos += FVector2D(0.f, 520.f);
			HandleButtonClicked(GActorBtn4, gswPos);
		}
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
	// 마녀의집
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
	// 잠수함
	else if (_itemObject->itemClass == ASM_ComputerGimmick::StaticClass())
	{
		gimmickSelectionWidget->bComputerInWorld = false;
	}
	else if (_itemObject->itemClass == ASM_PeriscopeGimmick::StaticClass())
	{
		gimmickSelectionWidget->bPeriscopeInWorld = false;
	}
	else if (_itemObject->itemClass == ASM_PressButtonGimmick::StaticClass())
	{
		gimmickSelectionWidget->bPressBtnInWorld = false;
	}
	else if (_itemObject->itemClass == ASM_WhistleGimmick::StaticClass())
	{
		gimmickSelectionWidget->bWhistleInWorld = false;
	}
	// 슈퍼마켓
	else if (_itemObject->itemClass == ASP_BottleGimmick::StaticClass())
	{
		gimmickSelectionWidget->bBottleInWorld = false;
	}
	else if (_itemObject->itemClass == ASP_CartGimmick::StaticClass())
	{
		gimmickSelectionWidget->bCartInWorld = false;
	}
	else if (_itemObject->itemClass == ASP_CleanerGimmick::StaticClass())
	{
		gimmickSelectionWidget->bCleanerInWorld = false;
	}
	else if (_itemObject->itemClass == ASP_ShowcaseGimmick::StaticClass())
	{
		gimmickSelectionWidget->bShowcaseInWorld = false;
	}
	// 금고
	else if (_itemObject->itemClass == ABS_GoldBarGimmick::StaticClass())
	{
		gimmickSelectionWidget->bGoldbarInWorld = false;
	}
	else if (_itemObject->itemClass == ABS_HandleGimmick::StaticClass())
	{
		gimmickSelectionWidget->bHandleInWorld = false;
	}
	else if (_itemObject->itemClass == ABS_LaserGimmick::StaticClass())
	{
		gimmickSelectionWidget->bLaserInWorld = false;
	}
	else if (_itemObject->itemClass == ABS_SwitchGimmick::StaticClass())
	{
		gimmickSelectionWidget->bSwitchInWorld = false;
	}

}

