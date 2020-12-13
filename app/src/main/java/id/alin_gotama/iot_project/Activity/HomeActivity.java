package id.alin_gotama.iot_project.Activity;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import id.alin_gotama.iot_project.Model.ServerDefaultResponse;
import id.alin_gotama.iot_project.R;
import id.alin_gotama.iot_project.Services.ApiClient;
import id.alin_gotama.iot_project.Services.services;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class HomeActivity extends AppCompatActivity implements View.OnClickListener {
    private Button btnOpen,btnClose;
    private TextView tvState;
    private RecyclerView recyclerView;
    private RecyclerView.LayoutManager layoutManager;
    private Boolean value;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);

        recyclerView = findViewById(R.id.rvHome);
        layoutManager = new LinearLayoutManager(this);
        recyclerView.setLayoutManager(layoutManager);
        btnClose = findViewById(R.id.btnHomeClose);
        btnOpen = findViewById(R.id.btnHomeOpen);
        this.tvState = findViewById(R.id.tvHomeState);

        this.btnOpen.setOnClickListener(this);
        this.btnClose.setOnClickListener(this);

        realTimeStateReader();
    }

    public void realTimeStateReader(){

        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                services mservices = ApiClient.getRetrofit().create(services.class);
                Call<ServerDefaultResponse> call = mservices.readGate();
                call.enqueue(new Callback<ServerDefaultResponse>() {
                    @Override
                    public void onResponse(Call<ServerDefaultResponse> call, Response<ServerDefaultResponse> response) {
                        String state = "UNKNOWN";
                        assert response.body() != null;
                        if(response.body().getState_right_now().matches("1")){
                            state = "OPEN";
                            HomeActivity.this.tvState.setText(state);
                        }else if(response.body().getState_right_now().matches("0")){
                            state = "CLOSE";
                            HomeActivity.this.tvState.setText(state);
                        }
                        HomeActivity.this.realTimeStateReader();
                        Log.d("loop","ohkey");
                    }

                    @Override
                    public void onFailure(Call<ServerDefaultResponse> call, Throwable t) {
                        HomeActivity.this.realTimeStateReader();
                        Log.d("loop","tidak ohkey");
                    }
                });
            }
        }, 3000);

    }

    @Override
    public void onClick(View v) {
    if(v.getId() == R.id.btnHomeOpen){
        Toast.makeText(this, "MASUK MAI", Toast.LENGTH_SHORT).show();
        services services = ApiClient.getRetrofit().create(services.class);
        Call<ServerDefaultResponse> call = services.openGate();

        call.enqueue(new Callback<ServerDefaultResponse>() {
            @Override
            public void onResponse(Call<ServerDefaultResponse> call, Response<ServerDefaultResponse> response) {
                Toast.makeText(HomeActivity.this, response.body().getMessage(), Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onFailure(Call<ServerDefaultResponse> call, Throwable t) {
                Toast.makeText(HomeActivity.this, "PERIKSA KONEKSI ANDA !", Toast.LENGTH_SHORT).show();
            }
        });
    }
    else if(v.getId() == R.id.btnHomeClose){
        services services = ApiClient.getRetrofit().create(services.class);
        Call<ServerDefaultResponse> call = services.closeGate();

        call.enqueue(new Callback<ServerDefaultResponse>() {
            @Override
            public void onResponse(Call<ServerDefaultResponse> call, Response<ServerDefaultResponse> response) {
                Toast.makeText(HomeActivity.this, response.body().getMessage(), Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onFailure(Call<ServerDefaultResponse> call, Throwable t) {
                Toast.makeText(HomeActivity.this, "PERIKSA KONEKSI ANDA !", Toast.LENGTH_SHORT).show();
            }
        });
    }
    }
}