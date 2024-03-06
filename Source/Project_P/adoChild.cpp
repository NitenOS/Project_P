// Fill out your copyright notice in the Description page of Project Settings.


#include "adoChild.h"

AadoChild::AadoChild() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AadoChild::BeginPlay() {
	Super::BeginPlay();
	waitTime = 5;
}

void AadoChild::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!navPoints.IsEmpty()) {
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Purple, FString(FString::SanitizeFloat(navPoints.Num())));
		if (navPoints[navPointCount] != FVector(0, 0, 0)) {
			setGoalLocation(FNavLocation(navPoints[navPointCount]));
		}

	}
}

void AadoChild::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
