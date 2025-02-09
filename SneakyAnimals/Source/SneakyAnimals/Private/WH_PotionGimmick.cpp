// Fill out your copyright notice in the Description page of Project Settings.


#include "WH_PotionGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "ItemObject.h"
#include "TestPlayer.h"
#include "Kismet/GameplayStatics.h"

AWH_PotionGimmick::AWH_PotionGimmick()
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


void AWH_PotionGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

	WorldContextObject = this;

}

void AWH_PotionGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (lerpTime > 1)
	{
		lerpTime = 0;
		GetWorldTimerManager().PauseTimer(boomT);
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


UItemObject* AWH_PotionGimmick::GetDefaultItemObject()
{

	UE_LOG(LogTemp, Warning, TEXT("my pos : (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(4, 2);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = AWH_PotionGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(50130.f, -50100.f, -913.f);
		newItemObject->beginRot = FRotator(0.f, -90.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
		newItemObject->bIsDraggable = true;
	}
	return newItemObject;
}

int32 AWH_PotionGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (Myactivetype)
	{
	case 0:
		Extincion(ActivePlayer);
		break;
	case 1:
		SelfExplosion(ActivePlayer);
		break;
	case 2:
		knowledgeinjection();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void AWH_PotionGimmick::Extincion(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : Extincion"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[0], GetActorLocation());
	
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->Death_EndMan();
		player->DeathCounting();
	}
}

void AWH_PotionGimmick::SelfExplosion(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : SelfExplosion"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[1], GetActorLocation());

	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		if (FVector::Dist(GetActorLocation(), it->GetActorLocation()) < 500.0)
		{
			it->Death_Homerun((ActivePlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal(), 500.0);
			it->DeathCounting();
		}
	}
}

void AWH_PotionGimmick::knowledgeinjection()
{
	bCanActive = false;

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[2], GetActorLocation());

	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}

//void AWH_PotionGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (bIsFinished)
//	{
//		return;
//	}
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
//void AWH_PotionGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
//}
