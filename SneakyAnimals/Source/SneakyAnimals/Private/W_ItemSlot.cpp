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
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>

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
	playerCharacter->SetActorLocation(FVector(2000, 0, 0));

	AUITestCharacter* uiTestPlayer = Cast<AUITestCharacter>(playerCharacter);


	if (!uiTestPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("uiTestPlayer is nullptr"));
	}

	if (uiTestPlayer)
	{
		itemComponent = uiTestPlayer->FindComponentByClass<UItemComponent>();

		UE_LOG(LogTemp, Warning, TEXT("uiTestPlayer connect itemComponent"));
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

}

void UW_ItemSlot::OnItemBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Item button clicked!"));

	SetCurrentCost();
	SpawnBookshelfGimmick();

	if (this->itemType == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("this itemtype is 0"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("this itemtype is not 0"));
	}
	//findBookshelfGimmick->GetDefaultItemObject();

	//if (itemComponent && itemObject)
	//{
	//	
	//	if (itemComponent->TryAddItem(itemObject))
	//	{
	//		int newCost = mapCustomWidget->maxCostAsInt(itemCostAsInt);
	//		mapCustomWidget->UpdateMaxCost(newCost * -1);
	//		//UE_LOG(LogTemp, Warning, TEXT("Ready to Add Item"));
	//	}
	//	//UE_LOG(LogTemp, Warning, TEXT("Object is Same"));
	//}


	// gridWidget->Refresh();


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

void UW_ItemSlot::SpawnBookshelfGimmick()
{
	FActorSpawnParameters SpawnParams;

	FVector SpawnLocation = FVector(0.f, 0.f, 100.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FTransform SpawnTransform;

	// AWH_BookshelfGimmick* TempSpawnActor = GetWorld()->SpawnActor<AWH_BookshelfGimmick>(ShelfG, SpawnLocation, SpawnRotation, SpawnParams);

	for (TActorIterator<AWH_BookshelfGimmick> It(GetWorld()); It; ++It)
	{
		AWH_BookshelfGimmick* bookShelfG = *It;

		FString newName = "";

		if (bookShelfG)
		{
			// MyActiveType에 따라 이름 설정
			switch (bookShelfG->Myactivetype)
			{
			case 0:
			{
				// 다같이 죽는 기믹
				newName = TEXT("bookShelf_G1");
				break;
			}
			case 1:
			{
				// 실행 시킨 사람만 죽는 기믹
				newName = TEXT("bookShelf_G2");
				break;
			}
			case 2:
			{
				// 레벨 클리어 기믹
				newName = TEXT("bookShelf_G3");
				break;
			}
			default:
			{
				newName = TEXT("bookShelf_GNULL");
				break;
			}
			}

			bookShelfG->Rename(*newName);
		}


		switch (itemType)
		{
		case 0:
		{
			////AWH_BookshelfGimmick* wh1 = GetWorld()->SpawnActor<AWH_BookshelfGimmick>(ShelfG, FVector(0, 0, 10), FRotator::ZeroRotator);
			//
			//AWH_BookshelfGimmick* wh1 = GetWorld()->SpawnActor<AWH_BookshelfGimmick>(AWH_BookshelfGimmick::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

			//itemObject = wh1->GetDefaultItemObject();
			///*
			//if (ItemTexture000)
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("000000000000000000success"));
			//}
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("111111111111fail"));
			//}*/

			//// 싱글톤 패턴을 사용해 UItemManager 클래스의 인스턴스에 접근
			//// UItemManager* ItemManager = UItemManager::Get();

			//// if (ItemManager)
			//{
			//	if (itemComponent->TryAddItem(itemObject))
			//	{
			//		UE_LOG(LogTemp, Warning, TEXT("TryAddItem be Successed!"));
			//	}
			//	else
			//	{
			//		UE_LOG(LogTemp, Warning, TEXT("TryAddItem be Failed!"));
			//	}
			//}

			if (bookShelfG->GetName() == TEXT("bookShelf_G1"))
			{
				FVector currentLoc = bookShelfG->GetActorLocation();
				currentLoc.Z += 300;
				bookShelfG->SetActorLocation(currentLoc);
			}

			break;
		}

		case 1:
		{
			AWH_BroomstickGimmick* wh2 = GetWorld()->SpawnActor<AWH_BroomstickGimmick>(AWH_BroomstickGimmick::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

			itemObject = wh2->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				UE_LOG(LogTemp, Warning, TEXT("TryAddItem be Successed!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TryAddItem be Failed!"));
			}

			break;
		}
		case 2:
		{
			AWH_PotionGimmick* wh3 = GetWorld()->SpawnActor<AWH_PotionGimmick>(AWH_PotionGimmick::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

			itemObject = wh3->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				UE_LOG(LogTemp, Warning, TEXT("TryAddItem be Successed!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TryAddItem be Failed!"));
			}

			break;
		}
		case 3:
		{
			AWH_WitchCauldronGimmick* wh4 = GetWorld()->SpawnActor<AWH_WitchCauldronGimmick>(AWH_WitchCauldronGimmick::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

			itemObject = wh4->GetDefaultItemObject();

			if (itemComponent->TryAddItem(itemObject))
			{
				UE_LOG(LogTemp, Warning, TEXT("TryAddItem be Successed!"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TryAddItem be Failed!"));
			}

			break;
		}
		}
	}
	//if (UWorld* World = GetWorld())
	//{
	//	FActorSpawnParameters SpawnParams;

	//	FTransform SpawnTransform;

	//	// 액터 스폰
	//	//AWH_BookshelfGimmick* TempSpawnActor = GetWorld()->SpawnActor<AWH_BookshelfGimmick>(AWH_BookshelfGimmick::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
	//	AWH_BookshelfGimmick* TempSpawnActor = World->SpawnActorDeferred<AWH_BookshelfGimmick>(AWH_BookshelfGimmick::StaticClass(), SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	//	if(IsValid(TempSpawnActor))	
	//	{ 
	//		UE_LOG(LogTemp, Warning, TEXT("SpawnActor valid check")); 
	//		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, UKismetStringLibrary::Conv_VectorToString(TempSpawnActor->GetActorLocation()));
	//	}

	//	//if (SpawnedActor)
	//	//{
	//	//	AssociatedActor = SpawnedActor;
	//	//}

	if (AssociatedActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("bookshelf actor be Spawnd!"));

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

void ItemSlotBindGimmickActor(int32 _ItemType_)
{
	/*switch (_ItemType_)
	{
	case 0:
	{
		AWH_BookshelfGimmick* wh1 = GetWorld()->SpawnActor<AWH_BookshelfGimmick>(ShelfTest, FVector(0, 0, -50000), FRotator::ZeroRotator);
		itemObject = wh1->GetDefaultItemObject();
		itemComponent->TryAddItem(itemObject);
		break;
	}
	case 1:
	{
		AWH_BroomstickGimmick* wh2 = GetWorld()->SpawnActor<AWH_BroomstickGimmick>(ShelfTest, FVector(0, 0, -50000), FRotator::ZeroRotator);
		itemObject = wh2->GetDefaultItemObject();
		break;
	}
	case 2:
	{
		AWH_BookshelfGimmick* wh3 = GetWorld()->SpawnActor<AWH_BookshelfGimmick>(ShelfTest, FVector(0, 0, -50000), FRotator::ZeroRotator);
		itemObject = wh3->GetDefaultItemObject();
		break;
	}
	case 3:
	{
		AWH_BookshelfGimmick* wh4 = GetWorld()->SpawnActor<AWH_BookshelfGimmick>(ShelfTest, FVector(0, 0, -50000), FRotator::ZeroRotator);
		itemObject = wh4->GetDefaultItemObject();
		break;
	}
	default:
		break;
	}*/
}

