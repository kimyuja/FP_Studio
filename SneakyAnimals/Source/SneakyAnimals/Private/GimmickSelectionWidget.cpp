// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickSelectionWidget.h"
#include "Components/Button.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/PlayerController.h"
#include "NewGridWidget.h"
#include "WH_BookshelfGimmick.h"
#include "WH_BroomstickGimmick.h"
#include "WH_PotionGimmick.h"
#include "WH_WitchCauldronGimmick.h"
#include "Gimmick.h"
#include "ItemObject.h"
#include <MapCustomWidget.h>
#include <../../../../../../../Source/Runtime/SlateCore/Public/Widgets/InvalidateWidgetReason.h>
#include <../../../../../../../Source/Runtime/SlateCore/Public/Styling/SlateTypes.h>

void UGimmickSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gridWidget = CreateWidget<UNewGridWidget>(GetWorld(), newGridWidget);
	
	bUsedClearGimmick = false;

	UE_LOG(LogTemp, Warning, TEXT("Start Clear : %d"), bUsedClearGimmick);

	UpdateClearBtnState();


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

	SetVisibility(ESlateVisibility::Collapsed);

	if (buttonName == "GActorBtn1")
	{
		gridWidget->BindItemObjByBtn(AWH_BookshelfGimmick::StaticClass(), 0);
		bBookShelfInWorld = true;
	}
	else if (buttonName == "GActorBtn2")
	{
		gridWidget->BindItemObjByBtn(AWH_BroomstickGimmick::StaticClass(), 0);
		bBroomstickInWorld = true;
	}
	else if (buttonName == "GActorBtn3")
	{
		gridWidget->BindItemObjByBtn(AWH_PotionGimmick::StaticClass(), 0);
		bPotionInWorld = true;
	}
	else if (buttonName == "GActorBtn4")
	{
		gridWidget->BindItemObjByBtn(AWH_WitchCauldronGimmick::StaticClass(), 0);
		bPotInWorld = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
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

	if (buttonName == "GActorBtn1")
	{
		gridWidget->BindItemObjByBtn(AWH_BookshelfGimmick::StaticClass(), 1);
		bBookShelfInWorld = true;
	}
	else if (buttonName == "GActorBtn2")
	{
		gridWidget->BindItemObjByBtn(AWH_BroomstickGimmick::StaticClass(), 1);
		bBroomstickInWorld = true;
	}
	else if (buttonName == "GActorBtn3")
	{
		gridWidget->BindItemObjByBtn(AWH_PotionGimmick::StaticClass(), 1);
		bPotionInWorld = true;
	}
	else if (buttonName == "GActorBtn4")
	{
		gridWidget->BindItemObjByBtn(AWH_WitchCauldronGimmick::StaticClass(), 1);
		bPotInWorld = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
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

	if (buttonName == "GActorBtn1")
	{
		gridWidget->BindItemObjByBtn(AWH_BookshelfGimmick::StaticClass(), 2);
		bBookShelfInWorld = true;
	}
	else if (buttonName == "GActorBtn2")
	{
		gridWidget->BindItemObjByBtn(AWH_BroomstickGimmick::StaticClass(), 2);
		bBroomstickInWorld = true;
	}
	else if (buttonName == "GActorBtn3")
	{
		gridWidget->BindItemObjByBtn(AWH_PotionGimmick::StaticClass(), 2);
		bPotionInWorld = true;
	}
	else if (buttonName == "GActorBtn4")
	{
		gridWidget->BindItemObjByBtn(AWH_WitchCauldronGimmick::StaticClass(), 2);
		bPotInWorld = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
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
	itemObejct = _ItemObj;
}
