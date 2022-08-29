// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicActor.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
ABasicActor::ABasicActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* ABasicActor::GetStaticMesh()
{
	return nullptr;
}
