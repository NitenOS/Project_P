// Fill out your copyright notice in the Description page of Project Settings.


#include "adoChild.h"

AadoChild::AadoChild() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AadoChild::BeginPlay() {
	Super::BeginPlay();
	waitTime = adoWaitTime;
}

void AadoChild::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (count < adoWaitTime) { count += DeltaTime; }

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::SanitizeFloat(count));

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

}