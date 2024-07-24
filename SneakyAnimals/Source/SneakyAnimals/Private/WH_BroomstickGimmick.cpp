
// Fill out your copyright notice in the Description page of Project Settings.


#include "WH_BroomstickGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include "TestPlayer.h"
#include "ItemObject.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/EngineTypes.h>

AWH_BroomstickGimmick::AWH_BroomstickGimmick()
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
	activeObject->SetRelativeLocation(FVector(0,0,-150.0));
	activeObject->SetupAttachment(base);
	


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AGimmick::SetCanActiveF);
}


void AWH_BroomstickGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

}

void AWH_BroomstickGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (lerpTime > 1)
	{
		lerpTime = 0;
		GetWorldTimerManager().PauseTimer(broomSmashT);
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


UItemObject* AWH_BroomstickGimmick::GetDefaultItemObject()
{
	UE_LOG(LogTemp, Warning, TEXT("my pos : (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(1, 1);
		/*newItemObject->icon = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/YSM/ItemIconInstance/M_IconBookshelf"));
		newItemObject->iconRotated90 = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/YSM/ItemIconInstance/M_IconBookshelf_R90"));
		newItemObject->iconRotated180 = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/YSM/ItemIconInstance/M_IconBookshelf_R180"));
		newItemObject->iconRotated270 = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/YSM/ItemIconInstance/M_IconBookshelf_R270"));
		newItemObject->bRotated = false;*/
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = AWH_BroomstickGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(50130.f, -50100.f, -910.f);
	}
	return newItemObject;
}

int32 AWH_BroomstickGimmick::OnMyActive(AActor* ActivePlayer)
{
	
	if (bIsFinished)
	{
		return -1;
	}

	bCanActive = false;

	switch (activeType)
	{
	case 0:
		BroomSmash(ActivePlayer);
		break;
	case 1:
		PoorDriver(ActivePlayer);
		break;
	case 2:
		DoorBurst(ActivePlayer);
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void AWH_BroomstickGimmick::BroomSmash(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : BroomSmash"));
	lerpTime = 0;
	GetWorldTimerManager().SetTimer(broomSmashT, [&]()
		{
			float rot = FMath::Lerp(0.0, 360.0, lerpTime * 2.0);
			float rot2 = FMath::Lerp(0.0, 45.0, lerpTime * 2.0);
			float loc = FMath::Lerp(0.0, -50.0, lerpTime);
			//rot = FMath::Clamp(rot, 0, 90.0);
			activeObject->SetRelativeLocation(FVector(0, 0, loc));
			activeObject->SetRelativeRotation(FRotator(0, rot, rot2));
		}, 0.03f, true, 0);
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->Death_Homerun((player->GetActorLocation() - GetActorLocation()).GetSafeNormal());
		player->DeathCounting();
		player->bIsDie = true;
	}

	
}

void AWH_BroomstickGimmick::PoorDriver(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : PoorDriver"));
	activeObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	activeObject->SetRelativeRotation(FRotator(-90.0f,0,0));
	activeObject->SetRelativeLocation(FVector(-100, 0, -30));
	AttachToActor(ActivePlayer, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("broomRide"));
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->Death_PoorDrive(false);
		player->DeathCounting();
		player->bIsDie = true;
	}
	FTimerHandle broomBurstT;
	GetWorldTimerManager().SetTimer(broomBurstT, [&]()
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Destroy();
	}, 1.0, false, 0);
}

void AWH_BroomstickGimmick::DoorBurst(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
	activeObject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	activeObject->SetRelativeRotation(FRotator(-90.0f, 0, 0));
	activeObject->SetRelativeLocation(FVector(-100, 0, -30));
	AttachToActor(ActivePlayer, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("broomRide"));
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->Death_PoorDrive(true);
	}
	FTimerHandle broomBurstT;
	GetWorldTimerManager().SetTimer(broomBurstT, [&]()
		{
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Destroy();
		}, 1.0, false, 0);
}

//void AWH_BroomstickGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
//	if (player)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("overlap"));
//		player->bCanActive = true;
//		//OnMyActive(OtherActor);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("RTY"));
//	}
//}
//
//void AWH_BroomstickGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (bIsFinished)
//	{
//		return;
//	}
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
//}
