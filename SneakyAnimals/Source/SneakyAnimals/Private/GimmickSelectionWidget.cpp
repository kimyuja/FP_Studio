// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickSelectionWidget.h"
#include "Components/Button.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/PlayerController.h"
//#include "NewGridWidget.h"
#include "WH_BookshelfGimmick.h"
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
#include "Gimmick.h"
#include "ItemObject.h"
#include "ItemComponent.h"
#include <MapCustomWidget.h>
#include <../../../../../../../Source/Runtime/SlateCore/Public/Widgets/InvalidateWidgetReason.h>
#include <../../../../../../../Source/Runtime/SlateCore/Public/Styling/SlateTypes.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "TestPlayer.h"

void UGimmickSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// gridWidget = CreateWidget<UNewGridWidget>(GetWorld(), newGridWidget);

	bUsedClearGimmick = false;

	UE_LOG(LogTemp, Warning, TEXT("Start Clear : %d"), bUsedClearGimmick);

	UpdateClearBtnState();

	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (playerCharacter)
	{
		myPlayer = Cast<ATestPlayer>(playerCharacter);

		if (myPlayer)
		{
			itemComp = myPlayer->GetItemComponent();

			if (!itemComp)
			{
				UE_LOG(LogTemp, Error, TEXT("itemComp is null"));
				return;
			}
		}
	}
}

bool UGimmickSelectionWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (GimmickBtn1)
	{
		GimmickBtn1->OnClicked.AddDynamic(this, &UGimmickSelectionWidget::OnGimmickBtn1Clicked);
	}

	if (GimmickBtn2)
	{
		GimmickBtn2->OnClicked.AddDynamic(this, &UGimmickSelectionWidget::OnGimmickBtn2Clicked);
	}

	if (ClearBtn)
	{
		ClearBtn->OnClicked.AddDynamic(this, &UGimmickSelectionWidget::OnClearBtnClicked);
	}

	return true;
}

void UGimmickSelectionWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	OnCustomMouseEnter.Broadcast(InGeometry, InMouseEvent);
}

void UGimmickSelectionWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnCustomMouseLeave.Broadcast(InMouseEvent);
}

void UGimmickSelectionWidget::OnGimmickBtn1Clicked()
{
	UE_LOG(LogTemp, Warning, TEXT("clicked button name is %s"), *buttonName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("!! GIMMICK1 BTN CLICK -> SET ACTIVENUM : 0"));
	UE_LOG(LogTemp, Warning, TEXT("?? MYPLAYERNUM IS %d"), playerRandNum);


	SetVisibility(ESlateVisibility::Collapsed);
	// 마녀의집
	if (playerRandNum == 0)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(AWH_BookshelfGimmick::StaticClass(), 0);

			bBookShelfInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(AWH_BroomstickGimmick::StaticClass(), 0);
			bBroomstickInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(AWH_PotionGimmick::StaticClass(), 0);
			bPotionInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(AWH_WitchCauldronGimmick::StaticClass(), 0);
			bPotInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}
	// 잠수함
	else if (playerRandNum == 1)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(ASM_ComputerGimmick::StaticClass(), 0);
			bComputerInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(ASM_PeriscopeGimmick::StaticClass(), 0);
			bPeriscopeInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(ASM_PressButtonGimmick::StaticClass(), 0);
			bPressBtnInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(ASM_WhistleGimmick::StaticClass(), 0);
			bWhistleInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}
	// 슈퍼마켓
	else if (playerRandNum == 2)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(ASP_BottleGimmick::StaticClass(), 0);
			bBottleInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(ASP_CartGimmick::StaticClass(), 0);
			bCartInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(ASP_CleanerGimmick::StaticClass(), 0);
			bCleanerInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(ASP_ShowcaseGimmick::StaticClass(), 0);
			bShowcaseInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}
	// 금고
	else if (playerRandNum == 3)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(ABS_GoldBarGimmick::StaticClass(), 0);
			bGoldbarInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(ABS_HandleGimmick::StaticClass(), 0);
			bHandleInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(ABS_LaserGimmick::StaticClass(), 0);
			bLaserInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(ABS_SwitchGimmick::StaticClass(), 0);
			bSwitchInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}

	/*if (itemObejct)
	{
		UE_LOG(LogTemp, Warning, TEXT("!!!!!! yes"));
		itemObejct->rotationAngle = 0;
		itemObejct->rotationImgCheck = 0;
	}*/

}

void UGimmickSelectionWidget::OnGimmickBtn2Clicked()
{
	UE_LOG(LogTemp, Warning, TEXT("clicked button name is %s"), *buttonName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("!! GIMMICK2 BTN CLICK -> SET ACTIVENUM : 1"));

	SetVisibility(ESlateVisibility::Collapsed);

	// 마녀의집
	if (playerRandNum == 0)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(AWH_BookshelfGimmick::StaticClass(), 1);
			bBookShelfInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(AWH_BroomstickGimmick::StaticClass(), 1);
			bBroomstickInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(AWH_PotionGimmick::StaticClass(), 1);
			bPotionInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(AWH_WitchCauldronGimmick::StaticClass(), 1);
			bPotInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}
	// 잠수함
	else if (playerRandNum == 1)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(ASM_ComputerGimmick::StaticClass(), 1);
			bComputerInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(ASM_PeriscopeGimmick::StaticClass(), 1);
			bPeriscopeInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(ASM_PressButtonGimmick::StaticClass(), 1);
			bPressBtnInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(ASM_WhistleGimmick::StaticClass(), 1);
			bWhistleInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}
	// 슈퍼마켓
	else if (playerRandNum == 2)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(ASP_BottleGimmick::StaticClass(), 1);
			bBottleInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(ASP_CartGimmick::StaticClass(), 1);
			bCartInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(ASP_CleanerGimmick::StaticClass(), 1);
			bCleanerInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(ASP_ShowcaseGimmick::StaticClass(), 1);
			bShowcaseInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}
	// 금고
	else if (playerRandNum == 3)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(ABS_GoldBarGimmick::StaticClass(), 1);
			bGoldbarInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(ABS_HandleGimmick::StaticClass(), 1);
			bHandleInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(ABS_LaserGimmick::StaticClass(), 1);
			bLaserInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(ABS_SwitchGimmick::StaticClass(), 1);
			bSwitchInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}


	/*if (itemObejct)
	{
		UE_LOG(LogTemp, Warning, TEXT("!!!!!! yes"));
		itemObejct->rotationAngle = 0;
		itemObejct->rotationImgCheck = 0;
	}*/
}

void UGimmickSelectionWidget::OnClearBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("clicked button name is %s"), *buttonName.ToString());
	UE_LOG(LogTemp, Warning, TEXT("!! CLEAR BTN CLICK -> SET ACTIVENUM : 2"));

	SetVisibility(ESlateVisibility::Collapsed);

	// 마녀의집
	if (playerRandNum == 0)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(AWH_BookshelfGimmick::StaticClass(), 2);
			bBookShelfInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(AWH_BroomstickGimmick::StaticClass(), 2);
			bBroomstickInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(AWH_PotionGimmick::StaticClass(), 2);
			bPotionInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(AWH_WitchCauldronGimmick::StaticClass(), 2);
			bPotInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}
	// 잠수함
	else if (playerRandNum == 1)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(ASM_ComputerGimmick::StaticClass(), 2);
			bComputerInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(ASM_PeriscopeGimmick::StaticClass(), 2);
			bPeriscopeInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(ASM_PressButtonGimmick::StaticClass(), 2);
			bPressBtnInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(ASM_WhistleGimmick::StaticClass(), 2);
			bWhistleInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}
	// 슈퍼마켓
	else if (playerRandNum == 2)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(ASP_BottleGimmick::StaticClass(), 2);
			bBottleInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(ASP_CartGimmick::StaticClass(), 2);
			bCartInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(ASP_CleanerGimmick::StaticClass(), 2);
			bCleanerInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(ASP_ShowcaseGimmick::StaticClass(), 2);
			bShowcaseInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}
	// 금고
	else if (playerRandNum == 3)
	{
		if (buttonName == "GActorBtn1")
		{
			BindItemObjByBtn(ABS_GoldBarGimmick::StaticClass(), 2);
			bGoldbarInWorld = true;
		}
		else if (buttonName == "GActorBtn2")
		{
			BindItemObjByBtn(ABS_HandleGimmick::StaticClass(), 2);
			bHandleInWorld = true;
		}
		else if (buttonName == "GActorBtn3")
		{
			BindItemObjByBtn(ABS_LaserGimmick::StaticClass(), 2);
			bLaserInWorld = true;
		}
		else if (buttonName == "GActorBtn4")
		{
			BindItemObjByBtn(ABS_SwitchGimmick::StaticClass(), 2);
			bSwitchInWorld = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
		}
	}

	/*if (itemObejct)
	{
		UE_LOG(LogTemp, Warning, TEXT("!!!!!! yes"));
		itemObejct->rotationAngle = 0;
		itemObejct->rotationImgCheck = 0;
	}*/

	bUsedClearGimmick = true;
	UpdateClearBtnState();

}

void UGimmickSelectionWidget::BindBtnWithActiveType(FName _BtnName)
{
	buttonName = _BtnName;

	UE_LOG(LogTemp, Warning, TEXT("!! MY BUTTON NAME IS %s"), *buttonName.ToString());
}

void UGimmickSelectionWidget::UpdateClearBtnState()
{
	UE_LOG(LogTemp, Warning, TEXT("Clear : %d"), bUsedClearGimmick);
	ClearBtn->SetIsEnabled(!bUsedClearGimmick);

	FLinearColor buttonColor;

	if (!ClearBtn->GetIsEnabled())
	{
		UE_LOG(LogTemp, Warning, TEXT("!!! CLEAR BTN IS NOT ENABLED"));
		buttonColor = FLinearColor(0.f, 0.f, 0.f, 0.5f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("!!! CLEAR BTN IS ENABLED"));
		buttonColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	}

	ClearBtn->SetColorAndOpacity(buttonColor);

	Invalidate(EInvalidateWidget::LayoutAndVolatility);

}

void UGimmickSelectionWidget::SetItemObejct(UItemObject* _ItemObj)
{
	itemObject = _ItemObj;
}

void UGimmickSelectionWidget::SetPlayerRandNum(int _PlayerNum)
{
	playerRandNum = _PlayerNum;
}

void UGimmickSelectionWidget::BindItemObjByBtn(TSubclassOf<AGimmick> _GimmickClass, int32 _ActiveType)
{
	if (_GimmickClass)
	{
		// 마녀의집
		if (_GimmickClass->IsChildOf(AWH_BookshelfGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is AWH_BookshelfGimmick"));

			for (TActorIterator<AWH_BookshelfGimmick> It(GetWorld()); It; ++It)
			{
				bookShelfActorArr.Add((*It));
				//UE_LOG(LogTemp, Warning, TEXT("AWH_BookshelfGimmick's Name : %s, PTR : %p"), *It->GetActorNameOrLabel(), *It);
				//UE_LOG(LogTemp, Warning, TEXT("bookShelfActor %d loc (%f, %f, %f)"), (*It)->newItemObject->itemActiveType, (*It)->GetActorLocation().X, (*It)->GetActorLocation().Y, (*It)->GetActorLocation().Z);
			}

			//UE_LOG(LogTemp, Warning, TEXT("bookShelfActorArr Size : %d"), bookShelfActorArr.Num());

			for (AWH_BookshelfGimmick* bookShelfActor : bookShelfActorArr)
			{
				// bookShelfActorArr[i]
				// UE_LOG(LogTemp, Warning, TEXT("arr start!!"));

				if (bookShelfActor)
				{

					bookShelfActor->Myactivetype = _ActiveType;
					bookShelfActor->SetActiveType(_ActiveType);

					itemObject = bookShelfActor->GetDefaultItemObject();

					// bookShelfActor->SetActorLocation(worldPosition);


					// UE_LOG(LogTemp, Warning, TEXT("bookShelfActor pos in world : (%f, %f, %f"), worldPosition.X, worldPosition.Y, worldPosition.Z);
					/*FVector newLocation = GridToWorld(bookShelfActor->GetGridX(), bookShelfActor->GetGridY());
					bookShelfActor->SetActorLocation(newLocation);*/

					// bookShelfActor->SetActorLocation(FVector(50130.f, -50100.f, -40.f));
					UE_LOG(LogTemp, Warning, TEXT("position changed"));


				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("bookShelfActor is null"));
				}
			}
		}
		else if (_GimmickClass->IsChildOf(AWH_BroomstickGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is AWH_BroomstickGimmick"));

			for (TActorIterator<AWH_BroomstickGimmick> It(GetWorld()); It; ++It)
			{
				broomStickActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("broomStickActorArr Num is %d"), broomStickActorArr.Num());

			for (AWH_BroomstickGimmick* broomStickActor : broomStickActorArr)
			{

				broomStickActor->Myactivetype = _ActiveType;
				broomStickActor->SetActiveType(_ActiveType);

				itemObject = broomStickActor->GetDefaultItemObject();


			}
		}
		else if (_GimmickClass->IsChildOf(AWH_PotionGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is AWH_PotionGimmick"));

			for (TActorIterator<AWH_PotionGimmick> It(GetWorld()); It; ++It)
			{
				potionActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("potionActorArr Num is %d"), potionActorArr.Num());

			for (AWH_PotionGimmick* potionActor : potionActorArr)
			{
				potionActor->Myactivetype = _ActiveType;
				potionActor->SetActiveType(_ActiveType);

				itemObject = potionActor->GetDefaultItemObject();


			}
		}
		else if (_GimmickClass->IsChildOf(AWH_WitchCauldronGimmick::StaticClass()))
		{
			for (TActorIterator<AWH_WitchCauldronGimmick> It(GetWorld()); It; ++It)
			{
				cauldronActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("cauldronActorArr Num is %d"), cauldronActorArr.Num());

			for (AWH_WitchCauldronGimmick* cauldronActor : cauldronActorArr)
			{
				// cauldronActor->newItemObject->itemActiveType = _ActiveType;
				cauldronActor->Myactivetype = _ActiveType;
				cauldronActor->SetActiveType(_ActiveType);

				itemObject = cauldronActor->GetDefaultItemObject();


			}
		}
		// 잠수함
		else if (_GimmickClass->IsChildOf(ASM_ComputerGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ASM_ComputerGimmick"));

			for (TActorIterator<ASM_ComputerGimmick> It(GetWorld()); It; ++It)
			{
				computerActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("computerActorArr Num is %d"), computerActorArr.Num());

			for (ASM_ComputerGimmick* computerActor : computerActorArr)
			{
				computerActor->Myactivetype = _ActiveType;
				computerActor->SetActiveType(_ActiveType);

				itemObject = computerActor->GetDefaultItemObject();
			}
		}
		else if (_GimmickClass->IsChildOf(ASM_PeriscopeGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ASM_PeriscopeGimmick"));

			for (TActorIterator<ASM_PeriscopeGimmick> It(GetWorld()); It; ++It)
			{
				periscopeActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("periscopeActorArr Num is %d"), periscopeActorArr.Num());

			for (ASM_PeriscopeGimmick* periscopeActor : periscopeActorArr)
			{
				periscopeActor->Myactivetype = _ActiveType;
				periscopeActor->SetActiveType(_ActiveType);

				itemObject = periscopeActor->GetDefaultItemObject();
			}
		}
		else if (_GimmickClass->IsChildOf(ASM_PressButtonGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ASM_PressButtonGimmick"));

			for (TActorIterator<ASM_PressButtonGimmick> It(GetWorld()); It; ++It)
			{
				pressbuttonActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("pressbuttonActorArr Num is %d"), pressbuttonActorArr.Num());

			for (ASM_PressButtonGimmick* pressbuttonActor : pressbuttonActorArr)
			{
				pressbuttonActor->Myactivetype = _ActiveType;
				pressbuttonActor->SetActiveType(_ActiveType);

				itemObject = pressbuttonActor->GetDefaultItemObject();
			}
		}
		else if (_GimmickClass->IsChildOf(ASM_WhistleGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ASM_WhistleGimmick"));

			for (TActorIterator<ASM_WhistleGimmick> It(GetWorld()); It; ++It)
			{
				whistleGimmickActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("whistleGimmickActorArr Num is %d"), whistleGimmickActorArr.Num());

			for (ASM_WhistleGimmick* whistleGimmickActor : whistleGimmickActorArr)
			{
				whistleGimmickActor->Myactivetype = _ActiveType;
				whistleGimmickActor->SetActiveType(_ActiveType);

				itemObject = whistleGimmickActor->GetDefaultItemObject();
			}
		}
		// 슈퍼마켓
		else if (_GimmickClass->IsChildOf(ASP_BottleGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ASP_BottleGimmick"));

			for (TActorIterator<ASP_BottleGimmick> It(GetWorld()); It; ++It)
			{
				bottleActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("bottleActorArr Num is %d"), bottleActorArr.Num());

			for (ASP_BottleGimmick* bottleActor : bottleActorArr)
			{
				bottleActor->Myactivetype = _ActiveType;
				bottleActor->SetActiveType(_ActiveType);

				itemObject = bottleActor->GetDefaultItemObject();
			}
		}
		else if (_GimmickClass->IsChildOf(ASP_CartGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ASP_CartGimmick"));

			for (TActorIterator<ASP_CartGimmick> It(GetWorld()); It; ++It)
			{
				cartActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("cartActorArr Num is %d"), cartActorArr.Num());

			for (ASP_CartGimmick* cartActor : cartActorArr)
			{
				cartActor->Myactivetype = _ActiveType;
				cartActor->SetActiveType(_ActiveType);

				itemObject = cartActor->GetDefaultItemObject();
			}
		}
		else if (_GimmickClass->IsChildOf(ASP_CleanerGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ASP_CleanerGimmick"));

			for (TActorIterator<ASP_CleanerGimmick> It(GetWorld()); It; ++It)
			{
				cleanerActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("cleanerActorArr Num is %d"), cleanerActorArr.Num());

			for (ASP_CleanerGimmick* cleanerActor : cleanerActorArr)
			{
				cleanerActor->Myactivetype = _ActiveType;
				cleanerActor->SetActiveType(_ActiveType);

				itemObject = cleanerActor->GetDefaultItemObject();
			}
		}
		else if (_GimmickClass->IsChildOf(ASP_ShowcaseGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ASP_ShowcaseGimmick"));

			for (TActorIterator<ASP_ShowcaseGimmick> It(GetWorld()); It; ++It)
			{
				showcaseActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("showcaseActorArr Num is %d"), showcaseActorArr.Num());

			for (ASP_ShowcaseGimmick* showcaseActor : showcaseActorArr)
			{
				showcaseActor->Myactivetype = _ActiveType;
				showcaseActor->SetActiveType(_ActiveType);

				itemObject = showcaseActor->GetDefaultItemObject();
			}
		}
		// 금고
		else if (_GimmickClass->IsChildOf(ABS_GoldBarGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ABS_GoldBarGimmick"));

			for (TActorIterator<ABS_GoldBarGimmick> It(GetWorld()); It; ++It)
			{
				goldbarActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("goldbarActorArr Num is %d"), goldbarActorArr.Num());

			for (ABS_GoldBarGimmick* goldbarActor : goldbarActorArr)
			{
				goldbarActor->Myactivetype = _ActiveType;
				goldbarActor->SetActiveType(_ActiveType);

				itemObject = goldbarActor->GetDefaultItemObject();
			}
		}
		else if (_GimmickClass->IsChildOf(ABS_HandleGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ABS_HandleGimmick"));

			for (TActorIterator<ABS_HandleGimmick> It(GetWorld()); It; ++It)
			{
				handleActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("handleActorArr Num is %d"), handleActorArr.Num());

			for (ABS_HandleGimmick* handleActor : handleActorArr)
			{
				handleActor->Myactivetype = _ActiveType;
				handleActor->SetActiveType(_ActiveType);

				itemObject = handleActor->GetDefaultItemObject();
			}
		}
		else if (_GimmickClass->IsChildOf(ABS_LaserGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ABS_LaserGimmick"));

			for (TActorIterator<ABS_LaserGimmick> It(GetWorld()); It; ++It)
			{
				laserActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("laserActorArr Num is %d"), laserActorArr.Num());

			for (ABS_LaserGimmick* laserActor : laserActorArr)
			{
				laserActor->Myactivetype = _ActiveType;
				laserActor->SetActiveType(_ActiveType);

				itemObject = laserActor->GetDefaultItemObject();
			}
		}
		else if (_GimmickClass->IsChildOf(ABS_SwitchGimmick::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("GimmickClass is ABS_SwitchGimmick"));

			for (TActorIterator<ABS_SwitchGimmick> It(GetWorld()); It; ++It)
			{
				switchActorArr.Add(*It);
			}
			UE_LOG(LogTemp, Warning, TEXT("switchActorArr Num is %d"), switchActorArr.Num());

			for (ABS_SwitchGimmick* switchActor : switchActorArr)
			{
				switchActor->Myactivetype = _ActiveType;
				switchActor->SetActiveType(_ActiveType);

				itemObject = switchActor->GetDefaultItemObject();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("!!! so sad"));
		}

		itemObject->rotationAngle = 0;

		itemComp->TryAddItem(itemObject);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("!!! GimmickClass is empty"));
	}

}
