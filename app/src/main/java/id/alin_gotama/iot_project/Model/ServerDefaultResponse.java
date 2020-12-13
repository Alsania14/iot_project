package id.alin_gotama.iot_project.Model;

import android.os.Parcel;
import android.os.Parcelable;

public class ServerDefaultResponse implements Parcelable {
    private String status;
    private String change_state_to;
    private String message;
    private String state_right_now;

    protected ServerDefaultResponse(Parcel in) {
        status = in.readString();
        change_state_to = in.readString();
        message = in.readString();
        state_right_now = in.readString();
    }

    public static final Creator<ServerDefaultResponse> CREATOR = new Creator<ServerDefaultResponse>() {
        @Override
        public ServerDefaultResponse createFromParcel(Parcel in) {
            return new ServerDefaultResponse(in);
        }

        @Override
        public ServerDefaultResponse[] newArray(int size) {
            return new ServerDefaultResponse[size];
        }
    };

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(status);
        dest.writeString(change_state_to);
        dest.writeString(message);
        dest.writeString(state_right_now);
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getChange_state_to() {
        return change_state_to;
    }

    public void setChange_state_to(String change_state_to) {
        this.change_state_to = change_state_to;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public String getState_right_now() {
        return state_right_now;
    }

    public void setState_right_now(String state_right_now) {
        this.state_right_now = state_right_now;
    }
}
