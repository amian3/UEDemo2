// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpActor.h"
#include "../BallPawn/BallPawn.h"
// Sets default values
AJumpActor::AJumpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));

//	JumpImpluse.X = 0.0f;;

}

// Called when the game starts or when spawned
void AJumpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJumpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJumpActor::NotifyHit(class UPrimitiveComponent* MyComp,
	class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(Other);
		if (PlayerBall)
		{
			PlayerBall->GetBall()->AddImpulse(JumpImpluse);
		}
	}
}


