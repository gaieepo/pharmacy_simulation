#ifndef Tech_h
#define Tech_h

#include "Prescription.h"

enum Status {
	SERVICE,
	IDLE,
};

class Tech {
public:
	inline Tech() {
		status = IDLE;
	}

	inline bool isIdle() const {
		return status == IDLE;
	}

	inline void serve(Prescription &prescription) {
		this->prescription = prescription;
	}

	inline void setBusy() {
		status = SERVICE;
	}

	inline void setIdle() {
		status = IDLE;
	}

	inline Prescription* getPrescription() {
		return &prescription;
	}
private:
	Prescription prescription;
	Status status;
};

#endif