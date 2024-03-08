// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "TimerManager.h" 
#include "childBase.generated.h"

UCLASS()
class PROJECT_P_API AchildBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AchildBase();

	// Simplifier Speed character
	/** Multiply the walk speed (base : 600.0f) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
		float walkSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
		TArray<FVector> navPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ChildDeplacement)
	float acceptedRadius = 100.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int oldNavPoint;
	FVector originPosition;
	FNavLocation goalLocation;
	AAIController* aiController;
	UNavigationSystemV1* navSystem;
	FTimerHandle TimerHandle;
	FPathFollowingRequestResult moveResult;

	int navPointCount = 1;
	float count = 0.0f;
	float waitTime = 1.0f;

	void OnMoveCompleted(EPathFollowingRequestResult::Type Result);
	virtual FPathFollowingRequestResult MoveIA(FNavLocation goalLocation);
	
	
	void moveChild();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FNavLocation getGoalLocation();
	void setGoalLocation(FNavLocation _goalLocation);
};
