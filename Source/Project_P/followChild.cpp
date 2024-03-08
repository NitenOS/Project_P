// Fill out your copyright notice in the Description page of Project Settings.


#include "followChild.h"

AfollowChild::AfollowChild() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AfollowChild::BeginPlay() {
	Super::BeginPlay();

	AActor* mainCharacter = UGameplayStatics::GetActorOfClass(GetWorld(), AtestCharacter::StaticClass());
	mc = Cast<AtestCharacter>(mainCharacter);
	FollowPlayer();
	moveChild();
}

void AfollowChild::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (moveResult == EPathFollowingRequestResult::AlreadyAtGoal)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString(TEXT("On a trouvé le MC")));
	FollowPlayer();
	moveResult = MoveIA(goalLocation);
}

bool AfollowChild::FollowPlayer() {
	bool isOnPlayer = false;

	setGoalLocation(FNavLocation(mc->GetActorLocation()));

	return isOnPlayer;
}