// Fill out your copyright notice in the Description page of Project Settings.


#include "SM_ComputerGimmick.h"

#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "TestPlayer.h"
#include "ItemObject.h"
#include "TrapDoor.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include "SM_ComputerMoniter.h"

ASM_ComputerGimmick::ASM_ComputerGimmick()
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

	planePanel1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane Panel1"));
	planePanel1->SetupAttachment(activeObject);
	planePanel1->SetRelativeScale3D(FVector(0.5, 0.3, 1));
	planePanel1->SetRelativeRotation(FRotator(0, 180, 90));
	planePanel1->SetRelativeLocation(FVector(-33, -5, 130.0));

	planePanel2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane Panel2"));
	planePanel2->SetupAttachment(activeObject);
	planePanel2->SetRelativeScale3D(FVector(0.5, 0.3, 1));
	planePanel2->SetRelativeRotation(FRotator(0, 180, 90));
	planePanel2->SetRelativeLocation(FVector(33, -5, 130.0));
	

	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGimmick::SetCanActiveT);
	trigger->OnComponentEndOverlap.AddDynamic(this, &AGimmick::SetCanActiveF);



	/*moniterUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("Emoticon UI"));
	moniterUI->SetupAttachment(activeObject);
	moniterUI->SetRelativeLocation(FVector(0, -3.6, 128.5));
	moniterUI->SetRelativeRotation(FRotator(0, -90.0, 0));
	moniterUI->SetDrawSize(FVector2D(115, 30));

	if (moniter)
	{
		moniterUI->SetWidgetClass(moniter);
	}*/
}


void ASM_ComputerGimmick::BeginPlay()
{
	Super::BeginPlay();

	SetActiveType(Myactivetype);
	planePanel1->SetVisibility(false);
	planePanel2->SetVisibility(false);
}

void ASM_ComputerGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	lerpTime += DeltaTime;

	if (lerpTime > 1)
	{
		lerpTime = 0;
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


UItemObject* ASM_ComputerGimmick::GetDefaultItemObject()
{
	newItemObject = NewObject<UItemObject>(this, UItemObject::StaticClass());

	if (newItemObject)
	{
		newItemObject->dimensions = FIntPoint(2, 2);
		newItemObject->rotationAngle = 0.0f;
		newItemObject->itemClass = ASM_ComputerGimmick::StaticClass();
		newItemObject->itemActiveType = Myactivetype;
		newItemObject->beginLoc = FVector(50000.f, 50000.f, -937.f);
		newItemObject->beginRot = FRotator(0.f, -90.f, 0.f);
		newItemObject->setWorldActorRot = 0;
		newItemObject->rotationImgCheck = 0;
	}
	return newItemObject;
}

int32 ASM_ComputerGimmick::OnMyActive(AActor* ActivePlayer)
{

	if (bIsFinished)
	{
		return -1;
	}

	switch (activeType)
	{
	case 0:
		Electrocution(ActivePlayer);
		break;
	case 1:
		SelfExplosion();
		break;
	case 2:
		SOS();
		break;
	default:
		break;
	}

	bIsFinished = true;

	return activeType;
}

void ASM_ComputerGimmick::Electrocution(AActor* ActivePlayer)
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 1 : Electrocution"));
	ATestPlayer* player = Cast<ATestPlayer>(ActivePlayer);
	if (player)
	{
		player->Death_Thunderclap();
		player->DeathCounting();
	}
}

void ASM_ComputerGimmick::SelfExplosion()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT(" Death 2 : SelfExplosion"));
	planePanel1->SetVisibility(true);
	planePanel2->SetVisibility(true);
	FTimerHandle hideT;
	GetWorldTimerManager().SetTimer(hideT, [&](){
		planePanel1->SetVisibility(false);
		planePanel2->SetVisibility(false);
	}, 1.0, false, 3.0);
	//Cast<USM_ComputerMoniter>(moniterUI->GetWidget())->SetWarning();
	for (TActorIterator<ATestPlayer> player(GetWorld()); player; ++player)
	{
		player->bIsDie = true;
		player->Respawn();
		player->DeathCounting();
	}
}

void ASM_ComputerGimmick::SOS()
{
	bCanActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Clear!"));
	//Cast<USM_ComputerMoniter>(moniterUI->GetWidget())->SetHelp();
}

//void ASM_ComputerGimmick::SetCanActiveT(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("RTY"));
//	}
//}
//
//void ASM_ComputerGimmick::SetCanActiveF(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	ATestPlayer* player = Cast<ATestPlayer>(OtherActor);
//	if (player)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("overlap"));
//		player->bCanActive = false;
//		player->g = nullptr;
//		bCanActive = false;
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("RTY"));
//	}
//}