// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructiveBlock.h"
#include "DestructibleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "../BallPawn/BallPawn.h"
// Sets default values
ADestructiveBlock::ADestructiveBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Block = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Block"));
	RootComponent = Block;

	for (int i = 0; i < 6; ++i)
	{
		Socket.Add(CreateDefaultSubobject<UStaticMeshComponent>(FName(*FString::FromInt(i))));
		Socket[i]->AttachTo(RootComponent);
	}
	for (int i = 0; i < 6; ++i)
	{
		Constraint.Add(CreateDefaultSubobject<UPhysicsConstraintComponent>(FName(*FString::FromInt(i + 6))));
		Constraint[i]->SetupAttachment(Socket[i]);
		Constraint[i]->SetWorldLocation(Socket[i]->GetComponentLocation());
		Constraint[i]->SetConstrainedComponents(Socket[i], Socket[i]->GetFName(), Block, Block->GetFName());
		Constraint[i]->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
		Constraint[i]->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
		Constraint[i]->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

	}
}

// Called when the game starts or when spawned
void ADestructiveBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructiveBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructiveBlock::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other,
	class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
	FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	//UE_LOG(LogTemp, Log, TEXT("NotifyHit"));

	if (Other && MyComp == Block)
	{
		ABallPawn* PlayerBall = Cast<ABallPawn>(Other);
		if (PlayerBall)
		{
			Block->ApplyDamage(10.0f, GetActorLocation(), GetActorForwardVector(), 10000.0f);
			if (!InDestroy)
			{
				DestroyTimer = DestroyBound;
				InDestroy = true;
			}

		}
	}
}