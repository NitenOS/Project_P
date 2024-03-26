// Fill out your copyright notice in the Description page of Project Settings.


#include "adoChild.h"

AadoChild::AadoChild() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	crySFX = CreateDefaultSubobject<UAudioComponent>(TEXT("CrySFX"));
	crySFX->SetRelativeLocation(FVector(0.0f, 0.0f, -70.0f));
	crySFX->SetupAttachment(RootComponent);
}

void AadoChild::BeginPlay() {
	Super::BeginPlay();
	waitTime = adoWaitTime;
}

void AadoChild::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (count < adoWaitTime) { count += DeltaTime; }

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::SanitizeFloat(count));

	if (count >= waitTime) {
		if (!navPoints.IsEmpty()) {

			oldNavPoint = navPointCount;

			do {
				navPointCount = FMath::RandRange(0, navPoints.Num() - 1);
			} while (navPointCount == oldNavPoint);

			this->SetActorLocation(navPoints[navPointCount]);
		}

		count = 0;
	}

	// Cry 
	{
		crySFX->Sound = cry[numCry];

		crycount += DeltaTime;
		if (crycount >= maxCryCount) {

			//numFootstep = FMath::RandRange(0, footsteps.Num() - 1);
			numCry += 1;
			if (numCry >= cry.Num()) { numCry = 0; }
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(numFootstep));

			crySFX->Play();
			crycount = 0.0f;
		}

	}
}