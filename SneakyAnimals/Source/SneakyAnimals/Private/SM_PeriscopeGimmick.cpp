// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_PeriscopeGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include "ItemObject.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "Kismet/GameplayStatics.h"

ASM_PeriscopeGimmick::ASM_PeriscopeGimmick()
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
	object->SetRelativeLocation(FVector(0, 0, 150.0));

	activeObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Active Object"));
	activeObject->SetupAttachment(object);
	activeObject->SetRelativeLocation(FVector(0, 0, -150.0));


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AGimmick::SetCanActiveF);
}


void ASM_PeriscopeGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

	WorldContextObject = this;

}

void ASM_PeriscopeGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (timerCheck >= 2.0)
	{
		GetWorldTimerManager().PauseTimer(clearSpinT);
		//GetWorldTimerManager().ClearAllTimersForObject(GetWorld());
		timerCheck = 0;
	}

	if (lerpTime > 1)
	{
		lerpTime = 0;
		GetWorldTimerManager().PauseTimer(spinT);
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

UItemObject* ASM_PeriscopeGimmick::GetDefaultItemObject()
{
	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(2, 2);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = ASM_PeriscopeGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(50000.f, 50000.f, -933.f);
		newItemObject->beginRot = FRotator(0.f, -90.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
		newItemObject->bIsDraggable = true;
	}
	return newItemObject;
}


int32 ASM_PeriscopeGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (Myactivetype)
	{
	case 0:
		PeriscopeSpin(ActivePlayer);
		break;
	case 1:
		ByeHandle(ActivePlayer);
		break;
	case 2:
		HandleShake();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ASM_PeriscopeGimmick::PeriscopeSpin(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : PeriscopeSpin"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[0], GetActorLocation());

	lerpTime = 0;
	GetWorldTimerManager().SetTimer(spinT, [&]()
		{
			float rot = FMath::Lerp(0, -90.0, lerpTime * 2);
			activeObject->SetRelativeRotation(FRotator(0 , rot , 0));
		}, 0.03f, true, 0);
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->ServerRPC_SetPlayerPhysics(player);
		player->bIsDie = true;
		player->Respawn();
		player->DeathCounting();
	}
	
}

void ASM_PeriscopeGimmick::ByeHandle(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : ByeHandle"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[1], GetActorLocation());

	activeObject->SetSimulatePhysics(true);
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->ServerRPC_SetPlayerPhysics(player);
		player->bIsDie = true;
		player->Respawn();
		player->DeathCounting();
	}
}

void ASM_PeriscopeGimmick::HandleShake()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[2], GetActorLocation());

	lerpTime = 0;
	GetWorldTimerManager().SetTimer(clearSpinT, [&]()
		{
			float rot = FMath::Lerp(0, 360.0, lerpTime);
			activeObject->SetRelativeRotation(FRotator(0, rot, 0));
			UE_LOG(LogTemp, Warning, TEXT("WWWWWWWWWWWWWWWWWW!   %f"),timerCheck);
			timerCheck += GetWorld()->GetDeltaSeconds();
		}, 0.03f, true, 0);

}

//void ASM_PeriscopeGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
//void ASM_PeriscopeGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
