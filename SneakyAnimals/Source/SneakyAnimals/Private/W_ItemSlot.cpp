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
#include "Kismet/GameplayStatics.h"

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
	playerCharacter->SetActorLocation(FVector(2000,0,0));

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
	
	//findBookshelfGimmick->GetDefaultItemObject();

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

	if (itemComponent && itemObject)
	{
		
		if (itemComponent->TryAddItem(itemObject))
		{
			int newCost = mapCustomWidget->maxCostAsInt(itemCostAsInt);
			mapCustomWidget->UpdateMaxCost(newCost * -1);
			//UE_LOG(LogTemp, Warning, TEXT("Ready to Add Item"));
		}
		//UE_LOG(LogTemp, Warning, TEXT("Object is Same"));
	}

	SpawnBookshelfGimmick();
	
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

	FVector SpawnLocation = FVector(100.0f, 100.0f, 100.0f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FTransform SpawnTransform;

	AWH_BookshelfGimmick* TempSpawnActor = GetWorld()->SpawnActor<AWH_BookshelfGimmick>(ShelfTest, SpawnLocation, SpawnRotation, SpawnParams);
	//if (UWorld* World = GetWorld())
	//{
	//	FActorSpawnParameters SpawnParams;

	//	

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

	//	//if (AssociatedActor != nullptr)
	//	//{
	//	//	UE_LOG(LogTemp, Warning, TEXT("bookshelf actor be Spawnd!"));
	//	//}

	//	//AssociatedActor->
	//}
}
