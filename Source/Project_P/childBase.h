// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "NavigationSystem.h" 
#include "childBase.generated.h"

UCLASS()
class PROJECT_P_API AchildBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AchildBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector originPosition;
	FNavLocation goalLocation;
	AAIController* aiController;
	UNavigationSystemV1* navSystem;

	void OnMoveCompleted(EPathFollowingRequestResult::Type Result);
	virtual FPathFollowingRequestResult MoveIA(FNavLocation goalLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
