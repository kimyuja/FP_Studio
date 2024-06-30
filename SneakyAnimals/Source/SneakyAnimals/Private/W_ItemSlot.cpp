// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ItemSlot.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "Slate/SlateBrushAsset.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "UITestCharacter.h"
#include "MapCustomWidget.h"
#include "Components/TextBlock.h"

bool UW_ItemSlot::Initialize()
{

	bool Success = Super::Initialize();
	if (!Success) return false;


	return true;
}

void UW_ItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	AUITestCharacter* uiTestPlayer = Cast<AUITestCharacter>(playerCharacter);

	if (itemBtn)
	{
		itemBtn->OnClicked.AddDynamic(this, &UW_ItemSlot::OnItemBtnClicked);
	}

	if (!mapCustomWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("mapCustomWidget is not set!"));
	}

}

void UW_ItemSlot::OnItemBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Item button clicked!"));

	FString itemCostText = itemCost->GetText().ToString();
	int itemCostAsInt = FCString::Atoi(*itemCostText);

	if (mapCustomWidget)
	{
		int newCost = mapCustomWidget->maxCostAsInt(itemCostAsInt);
		mapCustomWidget->UpdateMaxCost(newCost);
		UE_LOG(LogTemp, Warning, TEXT("Result: %d"), newCost);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("mapCustomWidget is null!"));
	}
}

void UW_ItemSlot::SetMapCustomWidget(UMapCustomWidget* Widget)
{
	mapCustomWidget = Widget;

	if (!mapCustomWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("mapCustomWidget is null!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("mapCustomWidget is set!"));
	}
}
