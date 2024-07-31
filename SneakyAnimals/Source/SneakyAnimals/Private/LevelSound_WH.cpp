// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSound_WH.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelSound_WH::ALevelSound_WH()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALevelSound_WH::BeginPlay()
{
	Super::BeginPlay();

	if (AudioComponent && AudioComponent->Sound)
	{
		AudioComponent->Play();
	}
	
}

// Called every frame
void ALevelSound_WH::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelSound_WH::FadeOutSound(float FadeOutDuration)
{
	if (AudioComponent)
	{
		AudioComponent->FadeOut(FadeOutDuration, 0.0f);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (AudioComponent)
				{
					AudioComponent->Stop();
				}
			}, 4.0f, false);

	}
}

