#include "Object.hpp"

#ifndef VAULTSERVER
#include "Game.hpp"
#endif

using namespace std;
using namespace RakNet;
using namespace Values;

#ifdef VAULTMP_DEBUG
DebugInput<Object> Object::debug;
#endif

Object::Object(unsigned int refID, unsigned int baseID) : Reference(refID, baseID)
{
	initialize();
}

Object::Object(const pDefault* packet) : Reference(PacketFactory::Pop<pPacket>(packet))
{
	initialize();

	string name;
	double X, Y, Z, aX, aY, aZ;
	unsigned int cell;
	bool enabled;
	unsigned int lock;
	unsigned int owner;

	PacketFactory::Access<pTypes::ID_OBJECT_NEW>(packet, name, X, Y, Z, aX, aY, aZ, cell, enabled, lock, owner);

	this->SetName(move(name));
	this->SetNetworkPos(Axis_X, X);
	this->SetNetworkPos(Axis_Y, Y);
	this->SetNetworkPos(Axis_Z, Z);
	this->SetAngle(Axis_X, aX);
	this->SetAngle(Axis_Y, aY);
	this->SetAngle(Axis_Z, aZ);
	this->SetNetworkCell(cell);
	this->SetEnabled(enabled);
	this->SetLockLevel(lock);
	this->SetOwner(owner);

	if (this->GetReference())
	{
		this->SetGamePos(Axis_X, X);
		this->SetGamePos(Axis_Y, Y);
		this->SetGamePos(Axis_Z, Z);
		this->SetGameCell(cell);
	}
}

Object::~Object() noexcept
{

}

void Object::initialize()
{
	for (const auto& axis : API::axis)
	{
		// emplace
		object_Game_Pos.insert(make_pair(axis.second, Value<double>()));
		object_Network_Pos.insert(make_pair(axis.second, Value<double>()));
		object_Angle.insert(make_pair(axis.second, Value<double>()));
	}

	this->SetLockLevel(Lock_Unlocked);
}

bool Object::IsValidCoordinate(double C)
{
	return (C != 2048.0 && C != 128.0 && C != 0.0);
}

bool Object::IsValidAngle(unsigned char axis, double A)
{
	return (axis == Axis_Z ? (A >= 0.0 && A <= 360.0) : (A >= -90.0 && A <= 90.0));
}

#ifndef VAULTSERVER
FuncParameter Object::CreateFunctor(unsigned int flags, NetworkID id)
{
	return FuncParameter(unique_ptr<VaultFunctor>(new ObjectFunctor(flags, id)));
}
#endif

const string& Object::GetName() const
{
	return *object_Name;
}

double Object::GetGamePos(unsigned char axis) const
{
	return object_Game_Pos.at(axis).get();
}

double Object::GetNetworkPos(unsigned char axis) const
{
	return object_Network_Pos.at(axis).get();
}

double Object::GetAngle(unsigned char axis) const
{
	return object_Angle.at(axis).get();
}

unsigned int Object::GetGameCell() const
{
	return cell_Game.get();
}

unsigned int Object::GetNetworkCell() const
{
	return cell_Network.get();
}

bool Object::GetEnabled() const
{
	return state_Enabled.get();
}

unsigned int Object::GetLockLevel() const
{
	return state_Lock.get();
}

unsigned int Object::GetOwner() const
{
	return state_Owner.get();
}

Lockable* Object::SetName(const string& name)
{
	return SetObjectValue(this->object_Name, name);
}

Lockable* Object::SetGamePos(unsigned char axis, double pos)
{
	if (!IsValidCoordinate(pos))
		return nullptr;

	return SetObjectValue(this->object_Game_Pos.at(axis), pos);
}

Lockable* Object::SetNetworkPos(unsigned char axis, double pos)
{
	if (!IsValidCoordinate(pos))
		return nullptr;

	return SetObjectValue(this->object_Network_Pos.at(axis), pos);
}

Lockable* Object::SetAngle(unsigned char axis, double angle)
{
	if (!IsValidAngle(axis, angle))
		return nullptr;

	return SetObjectValue(this->object_Angle.at(axis), angle);
}

Lockable* Object::SetGameCell(unsigned int cell)
{
	if (!cell)
		return nullptr;

	return SetObjectValue(this->cell_Game, cell);
}

Lockable* Object::SetNetworkCell(unsigned int cell)
{
	if (!cell)
		return nullptr;

	return SetObjectValue(this->cell_Network, cell);
}

Lockable* Object::SetEnabled(bool state)
{
	return SetObjectValue(this->state_Enabled, state);
}

Lockable* Object::SetLockLevel(unsigned int lock)
{
	return SetObjectValue(this->state_Lock, lock);
}

Lockable* Object::SetOwner(unsigned int owner)
{
	return SetObjectValue(this->state_Owner, owner);
}

VaultVector Object::vvec() const
{
	return VaultVector(GetGamePos(Axis_X), GetGamePos(Axis_Y), GetGamePos(Axis_Z));
}

bool Object::IsNearPoint(double X, double Y, double Z, double R) const
{
	return this->vvec().IsNearPoint(VaultVector(X, Y, Z), R);
}

bool Object::IsCoordinateInRange(unsigned char axis, double pos, double R) const
{
	return (GetGamePos(axis) > (pos - R) && GetGamePos(axis) < (pos + R));
}

pair<double, double> Object::GetOffset(double N) const
{
	return this->vvec().GetOffset(GetAngle(Axis_Z), N);
}

bool Object::HasValidCoordinates() const
{
	for (const auto& pos : object_Network_Pos)
		if (!IsValidCoordinate(pos.second.get()))
			return false;

	return true;
}

#ifdef VAULTSERVER
Lockable* Object::SetBase(unsigned int baseID)
{
	const DB::Record* record = *DB::Record::Lookup(baseID, vector<string>{"DOOR", "TERM", "STAT"});

	if (this->GetName().empty())
		this->SetName(record->GetDescription());

	return Reference::SetBase(baseID);
}
#endif

pPacket Object::toPacket() const
{
	pPacket pReferenceNew = Reference::toPacket();

	pPacket packet = PacketFactory::Create<pTypes::ID_OBJECT_NEW>(pReferenceNew, this->GetName(), this->GetNetworkPos(Values::Axis_X), this->GetNetworkPos(Values::Axis_Y), this->GetNetworkPos(Values::Axis_Z),
		this->GetAngle(Values::Axis_X), this->GetAngle(Values::Axis_Y), this->GetAngle(Values::Axis_Z), this->GetNetworkCell(), this->GetEnabled(), this->GetLockLevel(), this->GetOwner());

	return packet;
}

#ifndef VAULTSERVER
vector<string> ObjectFunctor::operator()()
{
	vector<string> result;
	NetworkID id = get();

	if (id)
		GameFactory::Operate<Object, FailPolicy::Return>(id, [this, &result](FactoryObject& object) {
			result.emplace_back(Utils::toString(object->GetReference()));
		});
	else
	{
		auto references = Game::GetContext(ID_OBJECT);

		for (unsigned int refID : references)
			GameFactory::Operate<Object, FailPolicy::Return>(refID, [this, refID, &result](FactoryObject& object) {
				if (!filter(object))
					result.emplace_back(Utils::toString(refID));
			});
	}

	_next(result);

	return result;
}

bool ObjectFunctor::filter(FactoryWrapper<Reference>& reference)
{
	return GameFactory::Operate<Object>(reference->GetNetworkID(), [this](FactoryObject& object) {
		unsigned int flags = this->flags();

		if (flags & FLAG_NOTSELF && object->GetReference() == PLAYER_REFERENCE)
			return true;

		else if (flags & FLAG_SELF && object->GetReference() != PLAYER_REFERENCE)
			return true;

		if (flags & FLAG_ENABLED && !object->GetEnabled())
			return true;

		return false;
	});
}
#endif
