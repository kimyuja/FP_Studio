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
#include <MapCustomWidget.h>

void UGimmickSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true; // 포커스를 받을 수 있도록 설정


	gridWidget = CreateWidget<UNewGridWidget>(GetWorld(), newGridWidget);

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

	if (buttonName == "GActorBtn1")
	{
		gridWidget->BindItemObjByBtn(AWH_BookshelfGimmick::StaticClass(), 0);
	}
	else if (buttonName == "GActorBtn2")
	{
		gridWidget->BindItemObjByBtn(AWH_BroomstickGimmick::StaticClass(), 0);
	}
	else if (buttonName == "GActorBtn3")
	{
		gridWidget->BindItemObjByBtn(AWH_PotionGimmick::StaticClass(), 0);
	}
	else if (buttonName == "GActorBtn4")
	{
		gridWidget->BindItemObjByBtn(AWH_WitchCauldronGimmick::StaticClass(), 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
	}

}

void UGimmickSelectionWidget::OnGimmickBtn2Clicked()
{
	UE_LOG(LogTemp, Warning, TEXT("clicked button name is %s"), *buttonName.ToString());

	if (buttonName == "GActorBtn1")
	{
		gridWidget->BindItemObjByBtn(AWH_BookshelfGimmick::StaticClass(), 1);
	}
	else if (buttonName == "GActorBtn2")
	{
		gridWidget->BindItemObjByBtn(AWH_BroomstickGimmick::StaticClass(), 1);
	}
	else if (buttonName == "GActorBtn3")
	{
		gridWidget->BindItemObjByBtn(AWH_PotionGimmick::StaticClass(), 1);
	}
	else if (buttonName == "GActorBtn4")
	{
		gridWidget->BindItemObjByBtn(AWH_WitchCauldronGimmick::StaticClass(), 1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
	}
}

void UGimmickSelectionWidget::OnClearBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("clicked button name is %s"), *buttonName.ToString());

	if (buttonName == "GActorBtn1")
	{
		gridWidget->BindItemObjByBtn(AWH_BookshelfGimmick::StaticClass(), 2);
	}
	else if (buttonName == "GActorBtn2")
	{
		gridWidget->BindItemObjByBtn(AWH_BroomstickGimmick::StaticClass(), 2);
	}
	else if (buttonName == "GActorBtn3")
	{
		gridWidget->BindItemObjByBtn(AWH_PotionGimmick::StaticClass(), 2);
	}
	else if (buttonName == "GActorBtn4")
	{
		gridWidget->BindItemObjByBtn(AWH_WitchCauldronGimmick::StaticClass(), 2);
	}
	else
	{	
		UE_LOG(LogTemp, Warning, TEXT("what is this btn..."));
	}

}

void UGimmickSelectionWidget::BindBtnWithActiveType(FName _BtnName)
{
	buttonName = _BtnName;
}
