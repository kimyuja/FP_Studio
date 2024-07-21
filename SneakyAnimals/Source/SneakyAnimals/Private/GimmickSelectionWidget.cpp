// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickSelectionWidget.h"
#include "Components/Button.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/PlayerController.h"

void UGimmickSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true; // 포커스를 받을 수 있도록 설정

    
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

void UGimmickSelectionWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	OnCustomMouseLeave.Broadcast(InMouseEvent);
}

void UGimmickSelectionWidget::OnGimmickBtn1Clicked()
{
    UE_LOG(LogTemp, Warning, TEXT("GimmickBtn1 clicked"));
}

void UGimmickSelectionWidget::OnGimmickBtn2Clicked()
{
    UE_LOG(LogTemp, Warning, TEXT("GimmickBtn2 clicked"));
}

void UGimmickSelectionWidget::OnClearBtnClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("ClearBtn clicked"));
}
