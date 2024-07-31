// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_CartGimmick.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "SP_RollingCart.h"
#include "GI_SneakyAnimals.h"
#include "ItemObject.h"

ASP_CartGimmick::ASP_CartGimmick()
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


void ASP_CartGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);

	//trigger->SetBoxExtent(activeRange);

}

void ASP_CartGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (!cartTarget && lerpTime > 1)
	{
		lerpTime = 0;
		
	}
	if ((cartTarget && FVector::Dist(GetActorLocation(), cartTarget->GetActorLocation()) < 100.0))
	{
		lerpTime = 0;
		UE_LOG(LogTemp, Warning, TEXT(" Boom!!!!!!!!!!!"));
		GetWorldTimerManager().ClearTimer(roadRollerT);
		cartTarget->bIsDie = true;
		cartTarget->Respawn();
		cartTarget->DeathCounting();
		Destroy();
	}
	if (Myactivetype == 2 && FVector::Dist(GetActorLocation(), FVector(0)) < 100.0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Go!!!!!!!!!!"));
		GetWorldTimerManager().ClearTimer(doorT);
		Destroy(true);
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


UItemObject* ASP_CartGimmick::GetDefaultItemObject()
{
	UE_LOG(LogTemp, Warning, TEXT("my pos : (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);

	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(2, 2);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = ASP_CartGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(-49813.f, 49355.f, -889.f);
		newItemObject->beginRot = FRotator(0.f, 180.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
	}
	return newItemObject;
}

int32 ASP_CartGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (Myactivetype)
	{
	case 0:
		Defective(ActivePlayer);
		break;
	case 1:
		RoadRoller(ActivePlayer);
		break;
	case 2:
		RollingCart();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ASP_CartGimmick::Defective(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : RoadRoller"));
	activeObject->SetSimulatePhysics(true);
	activeObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	activeObject->AddImpulse(FVector(0,0,10), TEXT(""), true);
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->bIsDie = true;
		player->Respawn();
		player->DeathCounting();
	}
}

void ASP_CartGimmick::RoadRoller(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : RoadRoller"));
	//GetWorld()->SpawnActor<ASP_RollingCart>(cart, GetActorLocation() + GetActorUpVector(), GetActorRotation());
	//if (ActivePlayer->HasAuthority())
	//{
	//	Cast<UGI_SneakyAnimals>(GetGameInstance())->GetRandomplayer();
	//	//cartTarget = Cast<UGI_SneakyAnimals>(GetGameInstance())->ranPlayer;
	//}
	//else
	//{
	//	//cartTarget = Cast<UGI_SneakyAnimals>(GetGameInstance())->ranPlayer;
	//}
	//for (TActorIterator<ASP_RollingCart> it(GetWorld()); it; ++it)
	//{
	//	it->target = cartTarget;
	//}
	cartTarget = Cast<ATestPlayer>(ActivePlayer);
	GetWorldTimerManager().SetTimer(roadRollerT, [&]()
		{
			FVector targetLoc = (cartTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			targetLoc = FVector(targetLoc.X, targetLoc.Y, 0);
			SetActorLocation(GetActorLocation() + targetLoc * 30.0);
			timerCheck += GetWorld()->GetDeltaSeconds();
		}, 0.03f, true, 0);
}

void ASP_CartGimmick::RollingCart()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
	/*GetWorldTimerManager().SetTimer(doorT, [&]()
		{
			FVector targetLoc = (FVector(0, 0, 0) - GetActorLocation()).GetSafeNormal();
			UE_LOG(LogTemp, Warning, TEXT("Door Boom!"));
			SetActorLocation(GetActorLocation() + targetLoc * 100.0);
			timerCheck += GetWorld()->GetDeltaSeconds();
		}, 0.03f, true, 0);*/
	lerpTime = 0;
}
