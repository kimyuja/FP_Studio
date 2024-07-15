// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ItemSlot.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>
#include "Slate/SlateBrushAsset.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Kismet/KismetStringLibrary.h"
#include "UITestCharacter.h"
#include "MapCustomWidget.h"
#include "ItemComponent.h"
#include "ItemObject.h"
#include "Components/TextBlock.h"
#include "WH_BookshelfGimmick.h"
#include "WH_BroomstickGimmick.h"
#include "WH_PotionGimmick.h"
#include "WH_WitchCauldronGimmick.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2D.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/AssetManager.h>
#include "W_CustomMap.h"
#include <ItemManager.h>
#include "NewGridWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <TestPlayer.h>

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
	if (!playerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("playerCharacter is nullptr"));
	}

	ATestPlayer* TestPlayer = Cast<ATestPlayer>(playerCharacter);


	if (!TestPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("TestPlayer is nullptr"));
	}

	if (TestPlayer)
	{
		itemComponent = TestPlayer->FindComponentByClass<UItemComponent>();

		UE_LOG(LogTemp, Warning, TEXT("TestPlayer connect itemComponent"));
	}

	if (itemBtn)
	{
		itemBtn->OnClicked.AddDynamic(this, &UW_ItemSlot::OnItemBtnClicked);
	}

	if (!mapCustomWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("mapCustomWidget is not set!"));
	}

	itemObject = NewObject<UItemObject>(this);

	if (!itemObject)
	{
		UE_LOG(LogTemp, Error, TEXT("itemObject is not set!"));
	}

	FindAllGimmick();

}

void UW_ItemSlot::OnItemBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Item button clicked!"));

	GimmickActorSetLoc();

	UE_LOG(LogTemp, Warning, TEXT("this itemtype is %d"), this->itemType);
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

void UW_ItemSlot::GimmickActorSetLoc()
{
	// UE_LOG(LogTemp, Error, TEXT("I'm Here"));

	switch (itemType)
	{
	case 0:
	{
		if (bookShelfActorArr[0])
		{
			FVector currentLoc = bookShelfActorArr[0]->GetActorLocation();
			currentLoc.Z += 300;
			bookShelfActorArr[0]->SetActorLocation(currentLoc);

			itemObject = bookShelfActorArr[0]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else if (bookShelfActorArr[1])
		{
			FVector currentLoc = bookShelfActorArr[1]->GetActorLocation();
			currentLoc.Z += 300;
			bookShelfActorArr[1]->SetActorLocation(currentLoc);

			itemObject = bookShelfActorArr[1]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else if (bookShelfActorArr[2])
		{
			FVector currentLoc = bookShelfActorArr[2]->GetActorLocation();
			currentLoc.Z += 300;
			bookShelfActorArr[2]->SetActorLocation(currentLoc);

			itemObject = bookShelfActorArr[2]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("bookShelfActorArr is NULL"));
		}
		break;
	}
	case 1:
	{
		if (broomStickActorArr[0])
		{
			FVector currentLoc = broomStickActorArr[0]->GetActorLocation();
			currentLoc.Z += 300;
			broomStickActorArr[0]->SetActorLocation(currentLoc);

			itemObject = broomStickActorArr[0]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else if (broomStickActorArr[1])
		{
			FVector currentLoc = broomStickActorArr[1]->GetActorLocation();
			currentLoc.Z += 300;
			broomStickActorArr[1]->SetActorLocation(currentLoc);

			itemObject = broomStickActorArr[1]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else if (broomStickActorArr[2])
		{
			FVector currentLoc = broomStickActorArr[2]->GetActorLocation();
			currentLoc.Z += 300;
			broomStickActorArr[2]->SetActorLocation(currentLoc);

			itemObject = broomStickActorArr[2]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("broomStickActorArr is NULL"));
		}
		break;
	}
	case 2:
	{
		if (potionActorArr[0])
		{
			FVector currentLoc = potionActorArr[0]->GetActorLocation();
			currentLoc.Z += 300;
			potionActorArr[0]->SetActorLocation(currentLoc);

			itemObject = potionActorArr[0]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else if (potionActorArr[1])
		{
			FVector currentLoc = potionActorArr[1]->GetActorLocation();
			currentLoc.Z += 300;
			potionActorArr[1]->SetActorLocation(currentLoc);

			itemObject = potionActorArr[1]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else if (potionActorArr[2])
		{
			FVector currentLoc = potionActorArr[2]->GetActorLocation();
			currentLoc.Z += 300;
			potionActorArr[2]->SetActorLocation(currentLoc);

			itemObject = potionActorArr[2]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("potionActorArr is NULL"));
		}
		break;
	}
	case 3:
	{
		if (cauldronActorArr[0])
		{
			FVector currentLoc = cauldronActorArr[0]->GetActorLocation();
			currentLoc.Z += 300;
			cauldronActorArr[0]->SetActorLocation(currentLoc);

			itemObject = cauldronActorArr[0]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else if (cauldronActorArr[1])
		{
			FVector currentLoc = cauldronActorArr[1]->GetActorLocation();
			currentLoc.Z += 300;
			cauldronActorArr[1]->SetActorLocation(currentLoc);

			itemObject = cauldronActorArr[1]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else if (cauldronActorArr[2])
		{
			FVector currentLoc = cauldronActorArr[2]->GetActorLocation();
			currentLoc.Z += 300;
			cauldronActorArr[2]->SetActorLocation(currentLoc);

			itemObject = cauldronActorArr[2]->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				// cost 변경
				SetCurrentCost();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("cauldronActorArr is NULL"));
		}
		break;
	}

	}

}

void UW_ItemSlot::SetItemIcon(const FString& TexturePath)
{
	// 텍스처 로드
	UTexture2D* ItemTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
	if (ItemTexture && itemIcon)
	{
		// 이미지 위젯의 브러시 설정
		itemIcon->SetBrushFromTexture(ItemTexture);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load texture or itemIcon is null"));
	}

}

void UW_ItemSlot::InitializeItemSlot(int32 _ItemType)
{
	switch (_ItemType)
	{
	case 0:
	{
		FString fromIcon1 = TEXT("/Game/RTY/Texture/Icon/bookShelf");
		SetItemIcon(fromIcon1);
		itemIcon->SetRenderScale(FVector2D(1.4f, 1.4f));
		itemCost->SetText(FText::FromString("3"));
		break;
	}
	case 1:
	{
		FString fromIcon2 = TEXT("/Game/RTY/Texture/Icon/broomStick.broomStick");
		SetItemIcon(fromIcon2);
		itemCost->SetText(FText::FromString("6"));
		break;
	}
	case 2:
	{
		FString fromIcon3 = TEXT("/Game/RTY/Texture/Icon/Table.Table");
		SetItemIcon(fromIcon3);
		itemCost->SetText(FText::FromString("4"));
		break;
	}
	case 3:
	{
		FString fromIcon4 = TEXT("/Game/RTY/Texture/Icon/pot.pot");
		SetItemIcon(fromIcon4);
		itemCost->SetText(FText::FromString("7"));
		break;
	}

	}
}
void UW_ItemSlot::SetCurrentCost()
{
	FString itemCostText = itemCost->GetText().ToString();
	int itemCostAsInt = FCString::Atoi(*itemCostText);

	// itemComponent->TryAddItem(itemObject);

	if (mapCustomWidget)
	{
		if (mapCustomWidget->ValidCost())
		{
			int newCost = mapCustomWidget->maxCostAsInt(itemCostAsInt);

			mapCustomWidget->UpdateMaxCost(newCost);
			UE_LOG(LogTemp, Warning, TEXT("Result: %d"), newCost);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("mapCustomWidget is null!"));
	}
}

void UW_ItemSlot::FindAllGimmick()
{
	// 마녀의 집

	for (TActorIterator<AWH_BookshelfGimmick> It(GetWorld()); It; ++It)
	{
		bookShelfActorArr.Add(*It);
	}

	for (TActorIterator<AWH_BroomstickGimmick> It(GetWorld()); It; ++It)
	{
		broomStickActorArr.Add(*It);
	}

	for (TActorIterator<AWH_PotionGimmick> It(GetWorld()); It; ++It)
	{
		potionActorArr.Add(*It);
	}

	for (TActorIterator<AWH_WitchCauldronGimmick> It(GetWorld()); It; ++It)
	{
		cauldronActorArr.Add(*It);
	}

	// 잠수함

	// 은행

	// 슈퍼마켓
}
