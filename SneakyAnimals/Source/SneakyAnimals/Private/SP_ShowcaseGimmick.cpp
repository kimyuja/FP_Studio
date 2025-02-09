// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_ShowcaseGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "SP_RollingCart.h"
#include "GI_SneakyAnimals.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include "ItemObject.h"
#include "Kismet/GameplayStatics.h"

ASP_ShowcaseGimmick::ASP_ShowcaseGimmick()
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
	object->SetRelativeLocation(FVector(0, 0, -150.0));

	activeObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Active Object"));
	activeObject->SetupAttachment(object);


	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AGimmick::SetCanActiveF);
}


void ASP_ShowcaseGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	WorldContextObject = this;

	//trigger->SetBoxExtent(activeRange);

}

void ASP_ShowcaseGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (lerpTime > 1)
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


UItemObject* ASP_ShowcaseGimmick::GetDefaultItemObject()
{
	UE_LOG(LogTemp, Warning, TEXT("my pos : (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(4, 4);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = ASP_ShowcaseGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(-49813.f, 49355.f, -889.f);
		newItemObject->beginRot = FRotator(0.f, -90.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
		newItemObject->bIsDraggable = true;
	}
	return newItemObject;
}

int32 ASP_ShowcaseGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (Myactivetype)
	{
	case 0:
		WaRRRR(ActivePlayer);
		break;
	case 1:
		Babo(ActivePlayer);
		break;
	case 2:
		MasterKey(ActivePlayer);
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ASP_ShowcaseGimmick::WaRRRR(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : WaRRRR"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[0], GetActorLocation());

	lerpTime = 0;
	GetWorldTimerManager().SetTimer(falloverT, [&]()
		{
			float rot = FMath::Lerp(0, 90.0, lerpTime);
			object->SetRelativeRotation(FRotator(rot, 0, 0));
		}, 0.03f, true, 0);
	for (TActorIterator<ATestPlayer> it(GetWorld()); it; ++it)
	{
		if (FVector::Dist(GetActorLocation(), it->GetActorLocation()) < 500.0)
		{
			GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [it]()
				{
					it->bIsDie = true;
					it->Death_Fallover();
					it->DeathCounting();
				}, 1.5f, false);
		}
	}
}

void ASP_ShowcaseGimmick::Babo(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : Babo"));

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[1], GetActorLocation());

	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, [player]()
			{
				player->ServerRPC_SetPlayerPhysics(player);
				player->bIsDie = true;
				player->Respawn();
				player->DeathCounting();
			}, 1.5f, false);
	}
}

void ASP_ShowcaseGimmick
::MasterKey(AActor* ActivePlayer)
{
	bCanActive = false;

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sounds[2], GetActorLocation());

	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
}
