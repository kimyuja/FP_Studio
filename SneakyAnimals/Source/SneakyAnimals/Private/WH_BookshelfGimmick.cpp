// Fill out your copyright notice in the Description page of Project Settings.


#include "WH_BookshelfGimmick.h"
#include "Gimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include "TestPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "FlyBook.h"
#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"
#include "ItemObject.h"
#include "Engine/Engine.h"

AWH_BookshelfGimmick::AWH_BookshelfGimmick()
{
	PrimaryActorTick.bCanEverTick = true;

	base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	SetRootComponent(base);

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	trigger->SetBoxExtent(FVector(150));
	trigger->SetupAttachment(base);
	trigger->SetCollisionObjectType(ECC_GameTraceChannel1);

	object = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object"));
	object->SetupAttachment(base);

	activeObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Active Object"));
	activeObject->SetupAttachment(base);
	activeObject->SetRelativeLocation(FVector(0, 0, -150.0));

	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AGimmick::SetCanActiveF);
}


void AWH_BookshelfGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);
	UE_LOG(LogTemp, Warning, TEXT("BookShelf Spawned at BeginPlay"));

	//trigger->SetBoxExtent(activeRange);

}

void AWH_BookshelfGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if(lerpTime > 1)
	{
		lerpTime = 0;
		GetWorldTimerManager().PauseTimer(falloverT);
	}
	

	if (bCanActive)
	{
		activeObject->SetRenderCustomDepth(true);

	}
	else
	{
		activeObject->SetRenderCustomDepth(false);

	}
}

// ¼±¹Î
UItemObject* AWH_BookshelfGimmick::GetDefaultItemObject()
{

	UE_LOG(LogTemp, Warning, TEXT("123131213131"));

	UItemObject* newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(3, 1);
		newItemObject->icon = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/RTY/Texture/Icon/bookShelf"));
		newItemObject->iconRotated90 = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/YSM/ItemIconInstance/M_IconBookshelf_R90"));
		newItemObject->iconRotated180 = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/YSM/ItemIconInstance/M_IconBookshelf_R180"));
		newItemObject->iconRotated270 = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/YSM/ItemIconInstance/M_IconBookshelf_R270"));
		newItemObject->bRotated = false;
		newItemObject->itemClass = AWH_BookshelfGimmick::StaticClass();
		UE_LOG(LogTemp, Warning, TEXT("987987987987987"));
	}
	return newItemObject;
}


int32 AWH_BookshelfGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), activeType);

	bCanActive = false;

	switch (activeType)
	{
	case 0:
		FallOver();
		break;
	case 1:
		BookCanFly();
		break;
	case 2:
		ButtonBook();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void AWH_BookshelfGimmick::FallOver()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : FallOver"));
	lerpTime = 0;
	GetWorldTimerManager().SetTimer(falloverT, [&]()
	{
		//activeObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		float rot = FMath::Lerp(0, -90.0, lerpTime);
		//float loc = FMath::Lerp(0.0, 50.0, lerpTime);
		//rot = FMath::Clamp(rot, 0, 90.0);
		//activeObject->SetRelativeLocation(FVector(0, 0, loc));
		activeObject->SetRelativeRotation(FRotator(0, 0, rot));
	}, 0.03f , true, 0);
	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		if (FVector::Dist(GetActorLocation(), it->GetActorLocation()) < 500.0)
		{
			it->Death_Fallover();
			it->DeathCounting();
		}
	}
}

void AWH_BookshelfGimmick::BookCanFly()
{
	bCanActive = false;
	GetWorld()->SpawnActor<AFlyBook>(flyBook, GetActorLocation() + GetActorUpVector() * 300.0f, GetActorRotation());
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : BookCanFly"));
}

void AWH_BookshelfGimmick::ButtonBook()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
	
}

//void AWH_BookshelfGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (bIsFinished)
//	{
//		return;
//	}
//	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
//	if (player)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("overlap"));
//		player -> bCanActive = true;
//		//OnMyActive(OtherActor);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("RTY"));
//	}
//}
//
//void AWH_BookshelfGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
//	if (player)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("overlap"));
//		player->bCanActive = false;
//		player->g = nullptr;
//		bCanActive = false;
//		//OnMyActive(OtherActor);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("RTY"));
//	}
//
//}

