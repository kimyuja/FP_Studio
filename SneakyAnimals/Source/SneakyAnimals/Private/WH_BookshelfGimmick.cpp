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
#include "Kismet/GameplayStatics.h"

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

	bReplicates = true;
	bNetLoadOnClient = true;
}


void AWH_BookshelfGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);
	//UE_LOG(LogTemp, Warning, TEXT("BookShelf Spawned at BeginPlay"));

	//trigger->SetBoxExtent(activeRange);
	
	WorldContextObject = this;

}

void AWH_BookshelfGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("My type : %d"), Myactivetype);
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

// ����
UItemObject* AWH_BookshelfGimmick::GetDefaultItemObject()
{
	//UE_LOG(LogTemp, Warning, TEXT("my pos : (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	// UItemObject* newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(4, 4);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = AWH_BookshelfGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(50130.f, -50100.f, -790.f);
		newItemObject->beginRot = FRotator(0.f, -180.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
		newItemObject->bIsDraggable = true;
	}
	return newItemObject;
}


int32 AWH_BookshelfGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), Myactivetype);

	bCanActive = false;

	switch (Myactivetype)
	{
	case 0:
		FallOver();
		break;
	case 1:
		BookCanFly(ActivePlayer);
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
			UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[0], GetActorLocation());

			it->bIsDie = true;
			it->Death_Fallover();
			it->DeathCounting();
		}
	}
}

void AWH_BookshelfGimmick::BookCanFly(AActor* ActivePlayer)
{
	bCanActive = false;
	AFlyBook* spawnedFlyBook = GetWorld()->SpawnActor<AFlyBook>(flyBook, GetActorLocation() + GetActorUpVector() * 300.0f, GetActorRotation());
	if (spawnedFlyBook)
	{
		UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[1], GetActorLocation());

		spawnedFlyBook->SetActivePlayer(ActivePlayer);
	}
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : BookCanFly"));
}

void AWH_BookshelfGimmick::ButtonBook()
{
	bCanActive = false;

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[2], GetActorLocation());

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

